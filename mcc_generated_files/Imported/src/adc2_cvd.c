/**
 * @file adc2_cvd.c
 * @brief PIC18F47K42 ADC2 Capacitive Voltage Divider (CVD) Driver Implementation
 * @details Implements register-level configurations for the Advanced ADC2
 *          peripheral to execute hardware-assisted CVD measurements.
 *          Datasheet Reference: Chapter 32 - Analog-to-Digital Converter with Computation (ADC2)
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "../adc2_cvd.h"
#include "../../system/system.h"

void ADC2_CVD_Initialize(void) {
    /* ------------------------------------------------------------------------
     * STEP 1: Turn ON ADC2 and Select FOSC Clock
     * Register: ADCON0 (ADC Control Register 0)
     * - ON = 1 (ADC enabled)
     * - CS = 0 (ADC clock source is FOSC, divided by ADCLK register)
     * ------------------------------------------------------------------------ */
    ADCON0bits.ON = 1;
    ADCON0bits.CS = 0; // FOSC is clock source

    /* ------------------------------------------------------------------------
     * STEP 2: Configure Clock Divider (TAD timing)
     * Register: ADCLK (ADC Clock Divider Register)
     * At FOSC = 64 MHz, we set ADCLK = 0x1F (divider = 64)
     * This yields: TAD = 64 / 64 MHz = 1.0 us
     * Datasheet recommendation: TAD must be between 1.0 us and 9.0 us.
     * ------------------------------------------------------------------------ */
    ADCLK = 0x1F; // Divide by 64

    /* ------------------------------------------------------------------------
     * STEP 3: Configure Voltage Reference
     * Register: ADREF (ADC Reference Selection Register)
     * - NREF = 0 (VREF- connected to VSS / Ground)
     * - PREF = 00 (VREF+ connected to VDD / 3.3V)
     * ------------------------------------------------------------------------ */
    ADREFbits.NREF = 0;
    ADREFbits.PREF = 0;

    /* ------------------------------------------------------------------------
     * STEP 4: Configure CVD Double Sampling & Polarities
     * Register: ADCON1 (ADC Control Register 1)
     * - ADDSEN = 1 (Enable Double Sampling Mode)
     * - ADPPOL = 0 (Precharge Polarity: Stage 1 internal VDD, external VSS;
     *                 Stage 2 internal VSS, external VDD)
     * - ADGPOL = 0 (Guard Polarity is active low / ground)
     * ------------------------------------------------------------------------ */
    ADCON1bits.ADDSEN = 1;  // Double sample enabled (critical for CVD!)
    ADCON1bits.ADPPOL = 1; // Stage 1: C_hold to VDD, Pin to VSS

    /* ------------------------------------------------------------------------
     * STEP 5: Configure Computation Engine Mode
     * Register: ADCON2 (ADC Control Register 2)
     * - ADMD = 010 (CVD/Average Mode)
     * - ADCRS = 000 (No division/shift of accumulator)
     * - ADPSIS = 0 (Accumulator is source for error calculation)
     * ------------------------------------------------------------------------ */
    ADCON2bits.ADMD = 0b010; // Average / CVD mode
    ADCON2bits.ADCRS = 0;    // No accumulator shift
    ADCON2bits.ADPSIS = 0;   // Error calculated from ADC result directly

    /* ------------------------------------------------------------------------
     * STEP 6: Configure Computation Interrupts and Thresholds
     * Register: ADCON3 (ADC Control Register 3)
     * - ADSOI = 0 (Continuous operation; do not stop on interrupt)
     * - ADTMD = 000 (Threshold interrupt is disabled)
     * ------------------------------------------------------------------------ */
    ADCON3bits.ADSOI = 0;
    ADCON3bits.ADTMD = 0;

    /* ------------------------------------------------------------------------
     * STEP 7: Configure Additional Sample & Hold Capacitor
     * Register: ADCAP (ADC Additional Sample-and-Hold Capacitor Register)
     * Adds internal capacitance to match larger external probe capacitances.
     * ADCAP = 0x07 adds 7 units (~8.4 pF) to the internal 10 pF C_hold.
     * ------------------------------------------------------------------------ */
    ADCAP = 0x07; 

    /* ------------------------------------------------------------------------
     * STEP 8: Configure CVD Timing Parameters
     * Registers: ADPRE (Precharge time) & ADACQ (Acquisition time)
     * - ADPRE = 16 (16 TAD = 16.0 us of precharge time to fully charge capacitors)
     * - ADACQ = 16 (16 TAD = 16.0 us of charge sharing acquisition time)
     * ------------------------------------------------------------------------ */
    ADPREH = 0x00;
    ADPREL = 0x10; // 16 TAD cycles precharge
    
    ADACQH = 0x00;
    ADACQL = 0x10; // 16 TAD cycles acquisition

    /* ------------------------------------------------------------------------
     * STEP 9: Configure Channel Selection
     * Register: ADPCH (ADC Positive Channel Selection Register)
     * Channel 0x00 corresponds to AN0 (RA0) pin.
     * ------------------------------------------------------------------------ */
    ADPCH = 0x00; // Select AN0
}

uint16_t ADC2_CVD_GetMeasurement(void) {
    // Force positive channel selection to AN0 (RA0)
    ADPCH = 0x00;

    // Reset accumulator & count registers prior to sampling
    ADCNT = 0;
    
    // Clear conversion complete interrupt flag (just in case)
    PIR1bits.ADTIF = 0;
    PIR1bits.ADIF = 0;

    // Start conversion by setting the GO/DONE bit
    // Datasheet Reference: ADCON0 register GO bit
    ADCON0bits.GO = 1;

    // Wait for the double sampling sequence to complete in hardware
    while (ADCON0bits.GO == 1);

    /* ------------------------------------------------------------------------
     * ERROR CALCULATION:
     * When ADDSEN = 1 and ADMD = 010 (CVD mode), the ADC2 hardware automatic
     * math engine performs:
     * 
     *      ADERR = Sample_1 - Sample_2
     * 
     * Since ADERR is a signed 16-bit register representing this differential,
     * we take its magnitude or bias it. For our capacitive tracking, we return
     * the raw ADERR register cast to uint16_t.
     * ------------------------------------------------------------------------ */
    int16_t diff_result = (int16_t)ADERR;
    
    // Convert signed difference to a stable positive 12-bit magnitude
    // Normally, Sample_1 (C_hold to VDD) > Sample_2 (C_hold to VSS).
    // So diff_result is positive. If it's negative due to noise, clamp to 0.
    if (diff_result < 0) {
        diff_result = 0;
    }
    
    return (uint16_t)diff_result;
}
