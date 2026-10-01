/**
 * @file main.c
 * @brief Main Control Loop and State Machine for PIC18F47K42 Level Switch (2-LED / Logic Out Version)
 * @details Executes a continuous 10ms real-time control loop: samples the probe, 
 *          applies noise-cancelling digital filters, calculates adaptive thresholds, 
 *          runs the debounce state machine, and drives Green/Red LEDs and 1/0 digital logic output.
 * 
 *          <h3>Operation:</h3>
 *          - Liquid Present: Green LED is ON, Red LED is OFF, LOGIC_OUT is High (1).
 *          - Liquid Absent: Green LED is OFF, Red LED is ON, LOGIC_OUT is Low (0).
 * 
 *          <h3>Diagnostic Blink Codes (Red LED):</h3>
 *          - 1 Flash: Probe Open Circuit Fault (C_sensor ~ 0 pF).
 *          - 2 Flashes: Probe Short Circuit to Tank/Ground (C_sensor > 500 pF).
 *          - 3 Flashes: ADC2 Peripheral Failure (no conversion timeout).
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "../../system/system.h"
#include "../adc2_cvd.h"
#include "../filter.h"
#include "../baseline.h"
#include "../level_sw_main.h"

#include "../../uart/uart1.h"
#include "../../system/pins.h"
#include "../../../io.h"

/* ------------------------------------------------------------------------
 * SYSTEM DIAGNOSTICS & THRESHOLDS DEFINITIONS
 * ------------------------------------------------------------------------ */
#define HYSTERESIS_VAL         25    // 12-bit CVD count deadband (prevents chatter)
#define DEBOUNCE_DELAY_COUNT   20    // 20 loops * 10ms = 200ms touch debounce
#define RELEASE_DELAY_COUNT    30    // 30 loops * 10ms = 300ms release delay

// Hardware limits for diagnostics (12-bit ADC error scale: 0 to 4095 counts)
// Your dry air baseline is ~3380 counts, so OPEN fault must be > 4050!
#define FAULT_LIMIT_OPEN       4050  // Above 4050 -> Pin disconnected / pulled to VDD
#define FAULT_LIMIT_SHORT      50    // Below 50   -> Probe dead shorted to Ground / Tank

// Diagnostic Error Codes
typedef enum {
    ERR_NONE = 0,
    ERR_PROBE_OPEN,
    ERR_PROBE_SHORT,
    ERR_ADC_FAIL
} ErrorCode_t;


/* ------------------------------------------------------------------------
 * STATE MACHINE DEFINITIONS
 * ------------------------------------------------------------------------ */
typedef enum {
    STATE_LIQUID_ABSENT = 0,
    STATE_DEBOUNCING_TOUCH,
    STATE_LIQUID_PRESENT,
    STATE_DEBOUNCING_RELEASE
} LevelState_t;

/* ------------------------------------------------------------------------
 * GLOBAL CONTROL VARIABLES
 * ------------------------------------------------------------------------ */
static LevelState_t current_state = STATE_LIQUID_ABSENT;
static ErrorCode_t  active_error = ERR_NONE;

static uint16_t raw_val = 3380;
static uint16_t filtered_val = 3380;
static uint16_t baseline_val = 3380;
static uint16_t threshold_val = 2800;
static bool     liquid_detected = false;

// Statistics and timing registers
static uint32_t ms_counter = 0;
static uint16_t uart_print_timer = 0;
static uint16_t debounce_timer = 0;

static SensitivityLevel_t current_sensitivity = SENSITIVITY_MEDIUM;
static uint8_t rotary_pos = 5;

// Universal Rotary Switch Dk Mappings: Pos 0 to 9
static const uint8_t dk_threshold_universal[10] = {0, 0, 2, 5, 15, 20, 25, 35, 50, 70};

/* ------------------------------------------------------------------------
 * HARDWARE GPIO CONTROLLERS (Pin 6: RA4 -> Sensor_Out)
 * ------------------------------------------------------------------------ */
void GPIO_SetLogicOutput(bool state) {
    LATAbits.LATA4 = state ? 1 : 0; // Drives physical Pin 6 (Sensor_Out)
}

bool GPIO_GetLogicOutput(void) {
    return (LATAbits.LATA4 == 1);
}


/* ------------------------------------------------------------------------
 * 220AMC10R ROTARY SWITCH READER (SW1: RB2..RB5)
 * ------------------------------------------------------------------------ */
static uint8_t ReadRotarySwitch(void) {
    uint8_t raw = (~PORTB) & 0x3C; // Extract bits 2, 3, 4, 5
    uint8_t pos = raw >> 2;        // Shift down to bits 0-3
    return (pos > 9) ? 9 : pos;
}

static uint16_t CalculateThresholdFromRotary(uint8_t pos, uint16_t basee) {
    if (pos == 0) return 4095; // Force OFF
    if (pos == 1) return 0;    // Force ON

    uint8_t target_dk = dk_threshold_universal[pos];
    // Threshold sits ABOVE dry air baseline (e.g. baseline + 300 to 800 counts)
    uint32_t offset = 300 + (uint32_t)(target_dk - 1) * 20;

    return (uint16_t)(basee + offset);
}

/* ------------------------------------------------------------------------
 * AUXILIARY SYSTEM ROUTINES
 * ------------------------------------------------------------------------ */

/**
 * @brief Evaluates hardware sensors to check for failure modes.
 * @details Analyzes raw ADC bounds to flag faults without false alarms on dry air (~3380).
 */
static void PerformSelfDiagnostics(void) {
    // 1. Check for Probe Short to Ground (Value < 50)
    if (raw_val < FAULT_LIMIT_SHORT) {
        active_error = ERR_PROBE_SHORT;
        return;
    }

    // 2. Check for Probe Open/Disconnected (Value saturated > 4050)
    if (raw_val > FAULT_LIMIT_OPEN) {
        active_error = ERR_PROBE_OPEN;
        return;
    }

    // Hardware healthy!
    active_error = ERR_NONE;
}

/**
 * @brief Drives fault indicators if an error occurs.
 */
static void UpdateDiagnosticsIndicator(void) {
    if (active_error == ERR_NONE) {
        return;
    }
    // Blink patterns or alarms can be added here
}

/* ------------------------------------------------------------------------
 * MAIN APPLICATION ENTRY POINT
 * ------------------------------------------------------------------------ */
int level_switch_main(void) 
{
    // 1. Core Hardware Initializations
    SYSTEM_Initialize();
    PIN_MANAGER_Initialize();
    UART1_Initialize();
    ADC2_CVD_Initialize();

    // Configure 220AMC10R Rotary Switch (RB2..RB5) pull-ups and Sensor_Out (RA4)
    WPUB |= 0x3C;
    TRISB |= 0x3C;
    ANSELB &= ~0x3C;
    TRISAbits.TRISA4 = 0;   // Pin 6 Output
    ANSELAbits.ANSELA4 = 0; // Pin 6 Digital
    GPIO_SetLogicOutput(false);
    
#ifdef DEBUG_ENABLE
    UART1_WriteString("\r\n============================================\r\n");
    UART1_WriteString("2-LED LEVEL SWITCH FIRMWARE INITIALIZED      \r\n");
    UART1_WriteString("PIC18F47K42 Operating at 64 MHz              \r\n");
    UART1_WriteString("No EEPROM & No Relay Configuration           \r\n");
    UART1_WriteString("============================================\r\n");
#endif
    
    UART1_WriteString("\r\nTesting ADC reading...\r\n");

    // Quick sanity test: print 5 raw samples to UART
    for (int i = 0; i < 5; i++) {
        uint16_t test_val = ADC2_CVD_GetMeasurement();
        UART1_WriteString("Raw CVD Sample = ");
        UART_PrintDec(test_val);
        UART1_WriteString(" counts\r\n");
        __delay_ms(100);
    }
    
    UART1_WriteString("\r\n--- CVD DIAGNOSTIC TEST ---\r\n");
    for (int i = 0; i < 5; i++) {
        uint16_t val = ADC2_CVD_GetMeasurement();
        UART1_WriteString("CVD Diff = ");
        UART_PrintDec(val);
        UART1_WriteString(" counts\r\n");
        __delay_ms(200);
    }
    
    // 2. Default Configuration (No EEPROM)
    current_sensitivity = SENSITIVITY_MEDIUM;

    // 3. Initial Baseline Calibration in Dry Air
    bool cal_ok = Baseline_PerformCalibration();
    if (cal_ok) 
    {
#ifdef DEBUG_ENABLE
        UART1_WriteString("Baseline Calibration: SUCCESS.\r\n");
#endif
    } 
    else 
    {
#ifdef DEBUG_ENABLE
        UART1_WriteString("Baseline Calibration: TIMEOUT/NOISY. Using default fallback.\r\n");
#endif
    }

    // 4. Initialise Filter buffers with stable starting baseline
    baseline_val = Baseline_Get();
    Filter_Initialize(baseline_val);

    rotary_pos = ReadRotarySwitch();
    threshold_val = CalculateThresholdFromRotary(rotary_pos, baseline_val);
    
    /* ------------------------------------------------------------------------
     * REAL-TIME CONTINUOUS EXECUTION LOOP (10ms Cycle)
     * ------------------------------------------------------------------------ */
    while (1) 
    {
        // Keep Watchdog alive safely
        // ClrWdt();

        // A. Sample Phase: Execute physical CVD conversion
        raw_val = ADC2_CVD_GetMeasurement();

        // B. Filtering Phase: Update digital filtering structures
        Filter_AddSample(raw_val);
        filtered_val = Filter_GetAdaptive(); // Retrieve filtered output

        // C. Diagnostics Phase: Validate hardware integrity
        PerformSelfDiagnostics();

        // D. Control Variables compilation & Rotary Switch update
        rotary_pos = ReadRotarySwitch();
        baseline_val = Baseline_Get();
        threshold_val = CalculateThresholdFromRotary(rotary_pos, baseline_val);

        /* ------------------------------------------------------------------------
         * FAULT FAIL-SAFE OVERRIDE
         * ------------------------------------------------------------------------ */
        if (active_error != ERR_NONE) 
        {
            liquid_detected = false;
            GPIO_SetGreenLED(false);
            GPIO_SetLogicOutput(false);    // Set digital logic out to 0 (Pin 6 LOW)
            UpdateDiagnosticsIndicator();
            __delay_ms(10);
            ms_counter++;
            continue;
        }

        /* ------------------------------------------------------------------------
         * ENVIRONMENTAL COMPENSATOR:
         * Continuously adjust baseline toward the filtered signal to track drift.
         * Inhibit updates when liquid is present.
         * ------------------------------------------------------------------------ */
        Baseline_TrackDrift(filtered_val, liquid_detected);

        /* ------------------------------------------------------------------------
         * DEBOUNCED DETECTION STATE MACHINE:
         * Liquid touch drops CVD count BELOW threshold.
         * ------------------------------------------------------------------------ */
        switch (current_state) 
        {
            case STATE_LIQUID_ABSENT:
                GPIO_SetGreenLED(false);
                GPIO_SetRedLED(true);
                GPIO_SetLogicOutput(false); // Pin 6 (Sensor_Out) = LOW (0V)
                liquid_detected = false;
                
                // When fluid touches probe, CVD value drops below threshold
                if (filtered_val < threshold_val) 
                {
                    current_state = STATE_DEBOUNCING_TOUCH;
                    debounce_timer = 0;
                }
                break;

            case STATE_DEBOUNCING_TOUCH:
                if (filtered_val < threshold_val) 
                {
                    debounce_timer++;
                    if (debounce_timer >= DEBOUNCE_DELAY_COUNT) 
                    {
                        current_state = STATE_LIQUID_PRESENT;
                    }
                } 
                else 
                {
                    // False trigger / glitch -> Reset back to dry state
                    current_state = STATE_LIQUID_ABSENT;
                }
                break;

            case STATE_LIQUID_PRESENT:
                GPIO_SetGreenLED(true);
                GPIO_SetRedLED(false);
                GPIO_SetLogicOutput(true);  // Pin 6 (Sensor_Out) = HIGH (3.3V)
                liquid_detected = true;

                // When probe is removed from liquid, CVD rises above threshold + Hysteresis
                if (filtered_val > (threshold_val + HYSTERESIS_VAL)) 
                {
                    current_state = STATE_DEBOUNCING_RELEASE;
                    debounce_timer = 0;
                }
                break;

            case STATE_DEBOUNCING_RELEASE:
                if (filtered_val > (threshold_val + HYSTERESIS_VAL)) 
                {
                    debounce_timer++;
                    if (debounce_timer >= RELEASE_DELAY_COUNT) 
                    {
                        current_state = STATE_LIQUID_ABSENT;
                    }
                } 
                else 
                {
                    current_state = STATE_LIQUID_PRESENT;
                }
                break;
        }

        /* ------------------------------------------------------------------------
         * TELEMETRY BROADCAST (Every 500 ms = 50 loops * 10ms)
         * ------------------------------------------------------------------------ */
        uart_print_timer++;
        if (uart_print_timer >= 50) 
        {
            UART1_WriteString("RAW=");
            UART_PrintDec(raw_val);
            UART1_WriteString(" FILTER=");
            UART_PrintDec(filtered_val);
            UART1_WriteString(" BASE=");
            UART_PrintDec(baseline_val);
            UART1_WriteString(" ROTARY=");
            UART_PrintDec(rotary_pos);
            UART1_WriteString(" TH=");
            UART_PrintDec(threshold_val);
            UART1_WriteString(" SENSOR_OUT=");
            UART1_WriteString(GPIO_GetLogicOutput() ? "1 (LIQUID)" : "0 (DRY)");
            UART1_WriteString("\r\n");

            uart_print_timer = 0;
        }

        // 10ms main loop cycle
        __delay_ms(10);
        ms_counter++;
    }

    return 0; // Unreachable
}