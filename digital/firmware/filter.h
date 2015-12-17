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

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// 8kHz Bandstop Chebyshev Filter Coefficients
static double bandstop_g = 0.9873701369842;
static double bandstop_num[3] = {1, -1.071992136563, 1};
static double bandstop_den[3] = {1, -1.058453022724, 0.9747402739684};
double bandstop_buf[3];


// 5kHz Low-pass Chebyshev Filter Coefficients
static double lowpass_g = 0.8912509381337;

static double lowpass_a_g = 0.08716604622656;
static double lowpass_a_num[3] = {1, 2, 1};
static double lowpass_a_den[3] = {1, -1.49955449681, 0.8482186817167};
double lowpass_a_buf[3];

static double lowpass_b_g = 0.02362756463502;
static double lowpass_b_num[3] = {1, 2, 1};
static double lowpass_b_den[3] = {1, -1.554785179597, 0.6492954381366};
double lowpass_b_buf[3];


double bandstop(double input);
double lowpass(double input);
double filter(double x, double *w, double *b, double *a);
void update_buffer(double *w);

#endif
