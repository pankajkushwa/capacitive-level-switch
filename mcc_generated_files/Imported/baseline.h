/**
 * @file baseline.h
 * @brief Automatic Baseline Calibration and Drift Tracking Header
 * @details Declares algorithms to calculate the initial sensor baseline in dry
 *          conditions and continuously track slow environmental drift
 *          (humidity, temperature, dust, and minor probe coating buildup).
 * 
 *          <h3>Baseline Tracking Safety Rules:</h3>
 *          1. <b>Inhibit on Liquid:</b> Tracking is strictly suspended when liquid is 
 *             active or when the signal is below the threshold, preventing the liquid
 *             value from being learned as "dry".
 *          2. <b>Slew-Rate Limiting:</b> Tracking operates with a massive low-pass constant,
 *             allowing only a maximum drift of 1 count per 10 seconds.
 *          3. <b>Stability Gate:</b> Baseline updates only occur if the variance of 
 *             the filtered CVD signal is below a tight tolerance window.
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef BASELINE_H
#define	BASELINE_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Number of samples to collect during initial power-on calibration.
 * @details 500 samples at 10ms intervals yields a robust 5-second initial calibration.
 */
#define CALIBRATION_SAMPLE_COUNT 500

/**
 * @brief Tolerance window for baseline tracking.
 * @details Signals beyond this gate from the current baseline are assumed to be
 *          either liquid touches or fault conditions and are ignored by tracking.
 */
#define BASELINE_TRACK_GATE 15

/**
 * @brief Standard Sensitivity definitions and their corresponding offsets
 *        subtracted from the Baseline to compute the detection threshold.
 */
typedef enum {
    SENSITIVITY_VERY_LOW = 0,   // Offset = 45 (Needs very large capacitance change)
    SENSITIVITY_LOW,            // Offset = 35
    SENSITIVITY_MEDIUM,         // Offset = 25 (Balanced default)
    SENSITIVITY_HIGH,           // Offset = 15
    SENSITIVITY_VERY_HIGH       // Offset = 10 (Highly sensitive to light chemical films)
} SensitivityLevel_t;

/**
 * @brief Runs the automatic startup calibration sequence.
 * @details Disables detection, flashes the Calibration LED, and averages 500 CVD samples.
 *          Verifies that the signal is stable.
 * @return true if calibration succeeded and is stable, false if too noisy (uses default).
 */
bool Baseline_PerformCalibration(void);

/**
 * @brief Slowly updates the baseline to compensate for temperature/humidity drift.
 * @details Evaluates the filtered value. If the signal is stable, dry, and within
 *          the tracking gate, it increments or decrements the tracking accumulator.
 * @param filtered_value The current digital filter output.
 * @param is_liquid_present Current detection state (used to inhibit tracking).
 */
void Baseline_TrackDrift(uint16_t filtered_value, bool is_liquid_present);

/**
 * @brief Gets the current operating Baseline value.
 * @return 12-bit baseline value.
 */
uint16_t Baseline_Get(void);

/**
 * @brief Manually forces a specific baseline value.
 * @param val The new baseline value.
 */
void Baseline_Set(uint16_t val);

/**
 * @brief Calculates the detection threshold based on current baseline and sensitivity.
 * @details Deducts the sensitivity-driven offset from the baseline.
 *          Formula: Threshold = Baseline - Sensitivity_Offset
 * @param sensitivity Selectable sensitivity level (0-4).
 * @return 12-bit trigger threshold.
 */
uint16_t Baseline_CalculateThreshold(SensitivityLevel_t sensitivity);

#endif	/* BASELINE_H */
