/**
 * @file filter.h
 * @brief Industrial Digital Filtering Module for Capacitive Level Switch
 * @details Declares digital filtering algorithms used to reject electrical and
 *          environmental noise from industrial motors, variable frequency drives (VFD),
 *          and high-voltage relay switching.
 * 
 *          <h3>Implemented Filtering Pipeline:</h3>
 *          1. <b>Median Filter (Size 5):</b> Sorts the latest 5 raw samples and extracts
 *             the middle value. Rejects short high-amplitude spike noise (e.g., VFD ignition, ESD).
 *          2. <b>Moving Average Filter (Size 8):</b> Smooths high-frequency Gaussian thermal noise.
 *          3. <b>IIR Low-Pass Filter:</b> Approximates an analog RC filter with exponential damping:
 *             
 *                 y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
 *             
 *          4. <b>Adaptive Rate Filter:</b> Varies alpha dynamically. If a rapid, sustained 
 *             change is detected (liquid touch), alpha is increased to achieve instant responsiveness.
 *             During steady-state (no touch), alpha is decreased to maximize noise rejection.
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef FILTER_H
#define	FILTER_H

#include <stdint.h>

/**
 * @brief Size of the Median Filter window.
 * @details Must be an odd number (5 is ideal for fast latency and spike rejection).
 */
#define MEDIAN_WINDOW_SIZE 5

/**
 * @brief Size of the Moving Average window.
 * @details Must be a power of 2 (e.g., 8) to allow fast division via bit-shifting in PIC.
 */
#define MOVING_AVG_SIZE 8

/**
 * @brief Initializes all filter structures and buffers to initial values.
 * @param initial_value The starting raw CVD measurement to populate buffers.
 */
void Filter_Initialize(uint16_t initial_value);

/**
 * @brief Feeds a raw CVD sample into the digital filtering pipeline.
 * @details Sequentially updates the median sorting buffer, moving average ring buffer,
 *          and calculates the IIR and Adaptive filter outputs.
 * @param raw_sample The latest raw CVD measurement from the ADC2.
 */
void Filter_AddSample(uint16_t raw_sample);

/**
 * @brief Gets the current Median Filter output.
 * @return 12-bit spike-free filtered value.
 */
uint16_t Filter_GetMedian(void);

/**
 * @brief Gets the current Moving Average Filter output.
 * @return 12-bit smoothed value.
 */
uint16_t Filter_GetMovingAverage(void);

/**
 * @brief Gets the current IIR Low-pass Filter output.
 * @return 12-bit exponentially-smoothed value.
 */
uint16_t Filter_GetIIR(void);

/**
 * @brief Gets the current Adaptive Filter output.
 * @details Best for final detection logic as it balances speed and stability.
 * @return 12-bit adaptive-smoothed value.
 */
uint16_t Filter_GetAdaptive(void);

#endif	/* FILTER_H */
