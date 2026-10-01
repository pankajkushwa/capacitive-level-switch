/**
 * @file adc2_cvd.h
 * @brief PIC18F47K42 ADC2 Capacitive Voltage Divider (CVD) Driver Header
 * @details This module configures the advanced ADC2 peripheral in hardware-assisted
 *          Capacitive Voltage Divider (CVD) mode for robust single-probe liquid sensing.
 * 
 *          <h3>CVD Mathematical Principles:</h3>
 *          CVD uses charge sharing between the external sensor capacitance (C_sensor)
 *          and the internal sample-and-hold capacitor (C_hold).
 *          
 *          Let V_hold be precharged to VDD (3.3V) and C_sensor discharged to VSS (0V).
 *          When connected in the acquisition phase, the final shared voltage V_shared is:
 *          
 *              V_shared = VDD * ( C_hold / (C_hold + C_sensor) )
 *          
 *          If liquid touches the probe, C_sensor increases. Consequently:
 *          - V_shared decreases in Sample A (where C_hold was charged to VDD).
 *          - V_shared increases in Sample B (where C_hold was charged to VSS).
 *          
 *          The hardware Double Sampling differential result is:
 *          
 *              CVD_Result = Sample_A - Sample_B
 *          
 *          This value is proportional to the inverse of C_sensor. A larger C_sensor
 *          results in a SMALLER CVD_Result.
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef ADC2_CVD_H
#define	ADC2_CVD_H

#include <xc.h>
#include <stdint.h>

/**
 * @brief Initializes the ADC2 peripheral specifically for CVD mode.
 * @details Configures the ADC clock, reference voltages, precharge polarity,
 *          additional S/H capacitance, precharge time (ADPRE), and acquisition
 *          time (ADACQ) registers.
 *          
 *          <h3>Timing Parameters (at 64 MHz System Clock):</h3>
 *          - ADCLK = 0x1F (ADCLK divider = 64) -> TAD = 1.0 us.
 *          - ADPRE = 16 TAD (16 us precharge): Fully charges/discharges C_hold and C_sensor.
 *          - ADACQ = 16 TAD (16 us acquisition): Guarantees complete charge transfer.
 *          - ADCAP = 0x07 (Internal tuning cap = 7 * 1.2 pF): Optimizes the charge sharing ratio.
 * 
 * @pre GPIO_Initialize must be called to set RA0 as Analog input.
 */
void ADC2_CVD_Initialize(void);

/**
 * @brief Executes a complete hardware-controlled CVD measurement.
 * @details Commands the ADC2 Computation Engine to execute a double-sampling CVD sequence.
 *          The first sample precharges C_hold to VDD and C_sensor to VSS.
 *          The second sample precharges C_hold to VSS and C_sensor to VDD.
 *          The PIC18F47K42 computation engine automatically performs subtraction
 *          and returns the differential voltage error.
 * 
 * @return 12-bit differential ADC result (0 to 4095). A drop in this value
 *         indicates an increase in probe capacitance (liquid presence).
 */
uint16_t ADC2_CVD_GetMeasurement(void);

#endif	/* ADC2_CVD_H */
