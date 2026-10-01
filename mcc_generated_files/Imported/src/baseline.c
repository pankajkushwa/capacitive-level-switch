/**
 * @file baseline.c
 * @brief Automatic Baseline Calibration and Drift Tracking Implementation
 * @details Implements 500-sample averaging for calibration and slow fractional 
 *          slew-rate-limited baseline tracking to compensate for drift.
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "../baseline.h"
#include "../adc2_cvd.h"
#include "../filter.h"
//#include "gpio.h"
#include "../../system/system.h"
#include <stdlib.h>


/* ------------------------------------------------------------------------
 * BASELINE STATIC VARIABLES
 * ------------------------------------------------------------------------ */
static uint16_t current_baseline = 500; // Default dry baseline estimate (e.g. ~500)

// Slew tracking accumulator. 
// Uses fractional tracking: updates the baseline only when accumulated drift
// exceeds a large integration factor to achieve massive time-constant filtering.
static int32_t  tracking_accumulator = 0;
#define INTEGRATION_LIMIT  500 // 500 calls * 10ms = 5 seconds of steady drift required for 1 count of adjustment

bool Baseline_PerformCalibration(void) {
    uint32_t sample_sum = 0;
    uint16_t min_sample = 0xFFFF;
    uint16_t max_sample = 0;
    uint16_t val;
    uint16_t i;

    // Indicate calibration starting
//    GPIO_SetCalibrationLED(true);
//    GPIO_SetRedLED(false);
//    GPIO_SetGreenLED(false);

    // Give hardware and power rails time to stabilize
    for (i = 0; i < 20; i++) {
        __delay_ms(10);
//        SYSTEM_WatchdogClear();
    }

    /* ------------------------------------------------------------------------
     * SAMPLE COLLECTION LOOP
     * Collects 500 samples, tracking min/max to verify noise levels.
     * ------------------------------------------------------------------------ */
    for (i = 0; i < CALIBRATION_SAMPLE_COUNT; i++) {
        val = ADC2_CVD_GetMeasurement();
        sample_sum += val;

        if (val < min_sample) min_sample = val;
        if (val > max_sample) max_sample = val;

        // Toggle calibration LED periodically to indicate activity
        if (i % 25 == 0) {
//            GPIO_ToggleCalibrationLED();
        }

        __delay_ms(10); // 10ms sampling interval
//        SYSTEM_WatchdogClear(); // Feed watchdog during calibration!
    }

    // Turn off calibration LED when complete
//    GPIO_SetCalibrationLED(false);

    /* ------------------------------------------------------------------------
     * VALIDATION & STABILITY CHECK
     * If the span (max - min) is excessive (e.g. > 45), the calibration environment
     * is deemed unstable/noisy (e.g., active VFD switching without proper ground).
     * ------------------------------------------------------------------------ */
    uint16_t span = max_sample - min_sample;
    if (span > 45) {
        // Calibration failed due to noise. Fall back to safe default baseline.
        current_baseline = 500;   // Safe default baseline on noisy startup
//        GPIO_SetRedLED(true);   // Signal a warning with Red LED
        return false;           // Report calibration failure
    }

    // Success: Calculate stable average and validate
    uint16_t avg = (uint16_t)(sample_sum / CALIBRATION_SAMPLE_COUNT);
    
    // Safety check: if average CVD is below 460, the probe is likely submerged in liquid at startup
    if (avg < 460) {
        current_baseline = 500;   // Keep standard dry default baseline
//        GPIO_SetRedLED(true);     // Signal warning/fault
        return false;             // Report calibration failure (submerged at boot)
    }

    current_baseline = avg;
    tracking_accumulator = 0;
    
    return true;
}

void Baseline_TrackDrift(uint16_t filtered_value, bool is_liquid_present) {
    // RULE 1: Never track drift while liquid is present!
    if (is_liquid_present) {
        tracking_accumulator = 0;
        return;
    }

    // Calculate distance from current baseline
    int16_t error = (int16_t)filtered_value - (int16_t)current_baseline;
    uint16_t abs_error = (uint16_t)abs(error);

    // RULE 2: If the signal is too far from baseline (exceeds Gate), ignore it.
    // This blocks tracking of rapid sensor changes or active touch-down transitions.
    if (abs_error > BASELINE_TRACK_GATE || abs_error == 0) {
        // Signal outside tracking gate or already perfect
        return;
    }

    // Integrate drift error (direction-sensitive)
    if (error > 0) {
        tracking_accumulator++;
    } else {
        tracking_accumulator--;
    }

    // RULE 3: Slew rate limiting.
    // Apply baseline update only when integration exceeds the limit.
    if (tracking_accumulator >= INTEGRATION_LIMIT) {
        current_baseline++;
        tracking_accumulator = 0;
    } else if (tracking_accumulator <= -INTEGRATION_LIMIT) {
        current_baseline--;
        tracking_accumulator = 0;
    }
}

uint16_t Baseline_Get(void) {
    return current_baseline;
}

void Baseline_Set(uint16_t val) {
    if (val <= 4000) {
        current_baseline = val;
    }
}

uint16_t Baseline_CalculateThreshold(SensitivityLevel_t sensitivity) {
    uint16_t offset = 15; // Default medium

    switch (sensitivity) {
        case SENSITIVITY_VERY_LOW:
            offset = 25;
            break;
        case SENSITIVITY_LOW:
            offset = 20;
            break;
        case SENSITIVITY_MEDIUM:
            offset = 15;
            break;
        case SENSITIVITY_HIGH:
            offset = 10;
            break;
        case SENSITIVITY_VERY_HIGH:
            offset = 5;
            break;
    }

    /* ------------------------------------------------------------------------
     * THRESHOLD MATHEMATICS (Negative Action Mode):
     * Liquid touch increases probe capacitance (C_sensor), which decreases the
     * raw CVD charge transfer value. Therefore, the liquid threshold lies 
     * BELOW the dry baseline.
     * ------------------------------------------------------------------------ */
    if (current_baseline > offset) {
        return current_baseline - offset;
    } else {
        return 50; // safe lower bound
    }
}
