/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 *
 * filter.h
 *
 */

#ifndef FILTER_H
#define FILTER_H

#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// 8kHz Bandstop Chebyshev Filter Coefficients
const double bandstop_num[3] = {1, -1.0719921366, 1};
const double bandstop_den[3] = {1, -1.0584530227, 0.97474027397};
const double bandstop_buf[3] = {0, 0, 0};


// 5kHz Low-pass Chebyshev Filter Coefficients
const double lowpass_a_num[3] = {1, 2, 1};
const double lowpass_a_den[3] = {1, -1.499554496, 0.84821868172};
const double lowpass_a_buf[3] = {0, 0, 0};

const double lowpass_b_num[3] = {1, 2, 1};
const double lowpass_b_den[3] = {1, -1.554785180, 0.64929543814};
const double lowpass_b_buf[3] = {0, 0, 0};


static double bandstop(double input);
static double lowpass(double input);
static double filter(double x, double *w, double *b, double *a);
void update_buffer(double *w);

#endif
