/**
 * @file filter.c
 * @brief Industrial Digital Filtering Module Implementation for PIC18F47K42
 * @details Implements Median, Moving Average, fixed-point IIR, and Adaptive
 *          filtering. All divisions are implemented via binary bit-shifting 
 *          to avoid floating-point library overhead on the 8-bit PIC18 MCU.
 * 
 * @author Senior Embedded Systems Engineer
 * @date 2026
 * 
 * @license SPDX-License-Identifier: Apache-2.0
 */

#include "../filter.h"
#include <stdlib.h>

/* ------------------------------------------------------------------------
 * FILTER STRUCTURES AND STATIC MEMORY
 * ------------------------------------------------------------------------ */
static uint16_t median_buffer[MEDIAN_WINDOW_SIZE];
static uint8_t  median_index = 0;

static uint16_t avg_buffer[MOVING_AVG_SIZE];
static uint32_t avg_sum = 0;
static uint8_t  avg_index = 0;

static uint32_t iir_accumulator = 0; // Fixed-point accumulator (scaled by 256)
static uint32_t adaptive_accumulator = 0;

// Filter Coefficients (represented as values out of 256 for fixed-point math)
#define IIR_ALPHA_STABLE     32    // 32/256 = 12.5% (Smooth)
#define ADAPTIVE_ALPHA_MIN   16    // 16/256 = 6.25% (Ultra-smooth for stable signals)
#define ADAPTIVE_ALPHA_MAX   128   // 128/256 = 50.0% (Fast tracking for transitions)
#define ADAPTIVE_THRESHOLD   10    // CVD delta threshold to switch to fast tracking (lowered for water/diesel sensitivity)

void Filter_Initialize(uint16_t initial_value) {
    uint8_t i;
    
    // Initialize Median buffer
    for (i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        median_buffer[i] = initial_value;
    }
    median_index = 0;

    // Initialize Moving Average buffer
    avg_sum = 0;
    for (i = 0; i < MOVING_AVG_SIZE; i++) {
        avg_buffer[i] = initial_value;
        avg_sum += initial_value;
    }
    avg_index = 0;

    // Initialize IIR accumulators (value scaled by 256 for 8-bit fraction)
    iir_accumulator = (uint32_t)initial_value << 8;
    adaptive_accumulator = (uint32_t)initial_value << 8;
}

void Filter_AddSample(uint16_t raw_sample) {
    /* ------------------------------------------------------------------------
     * 1. UPDATE MEDIAN FILTER BUFFER
     * ------------------------------------------------------------------------ */
    median_buffer[median_index] = raw_sample;
    median_index++;
    if (median_index >= MEDIAN_WINDOW_SIZE) {
        median_index = 0;
    }

    // Compute median to reject random transient spikes/drops
    uint16_t median_sample = Filter_GetMedian();

    /* ------------------------------------------------------------------------
     * 2. UPDATE MOVING AVERAGE BUFFER (Optimized running-sum method)
     * ------------------------------------------------------------------------ */
    avg_sum -= avg_buffer[avg_index];      // Subtract oldest sample
    avg_buffer[avg_index] = median_sample; // Store median sample
    avg_sum += median_sample;              // Add median sample to running sum
    
    avg_index++;
    if (avg_index >= MOVING_AVG_SIZE) {
        avg_index = 0;
    }

    /* ------------------------------------------------------------------------
     * 3. UPDATE FIXED-POINT IIR FILTER
     * Formula: IIR_out = (Alpha * current) + ((256 - Alpha) * IIR_out)
     * Implement division by 256 using bit-shift right by 8 (>> 8)
     * ------------------------------------------------------------------------ */
    uint32_t current_scaled = (uint32_t)median_sample << 8;
    
    iir_accumulator = ((uint32_t)IIR_ALPHA_STABLE * current_scaled + 
                      (256 - IIR_ALPHA_STABLE) * iir_accumulator) >> 8;

    /* ------------------------------------------------------------------------
     * 4. UPDATE ADAPTIVE RATE FILTER
     * Calculates the signal delta. If delta is large, alpha increases for response.
     * If delta is small, alpha decreases for extreme noise suppression.
     * ------------------------------------------------------------------------ */
    uint16_t current_adaptive_out = (uint16_t)(adaptive_accumulator >> 8);
    int16_t delta = (int16_t)median_sample - (int16_t)current_adaptive_out;
    uint16_t abs_delta = (uint16_t)abs(delta);
    
    uint16_t adaptive_alpha;
    if (abs_delta > ADAPTIVE_THRESHOLD) {
        // High rate of change detected -> Switch to maximum responsiveness
        adaptive_alpha = ADAPTIVE_ALPHA_MAX;
    } else {
        // Steady-state signal -> Switch to maximum noise dampening
        adaptive_alpha = ADAPTIVE_ALPHA_MIN;
    }

    adaptive_accumulator = ((uint32_t)adaptive_alpha * current_scaled + 
                           (256 - adaptive_alpha) * adaptive_accumulator) >> 8;
}

uint16_t Filter_GetMedian(void) {
    // Copy buffer to temporary array to avoid corrupting running filter
    uint16_t temp[MEDIAN_WINDOW_SIZE];
    uint8_t i, j;
    for (i = 0; i < MEDIAN_WINDOW_SIZE; i++) {
        temp[i] = median_buffer[i];
    }

    // Execute simple insertion sort (highly efficient for small N=5)
    for (i = 1; i < MEDIAN_WINDOW_SIZE; i++) {
        uint16_t key = temp[i];
        j = i;
        while (j > 0 && temp[j - 1] > key) {
            temp[j] = temp[j - 1];
            j--;
        }
        temp[j] = key;
    }

    // Return middle sorted element (Index 2 for size 5)
    return temp[2];
}

uint16_t Filter_GetMovingAverage(void) {
    // Division by 8 is optimized as bit-shift right by 3 (>> 3)
    return (uint16_t)(avg_sum >> 3);
}

uint16_t Filter_GetIIR(void) {
    // Scale down from fixed-point representation (divide by 256)
    return (uint16_t)(iir_accumulator >> 8);
}

uint16_t Filter_GetAdaptive(void) {
    return (uint16_t)(adaptive_accumulator >> 8);
}
