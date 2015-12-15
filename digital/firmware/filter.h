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

const float bandstop_num[3] = {1, -1.0719921366, 1};
const float bandstop_den[3] = {1, -1.0584530227, 0.97474027397};

const float lowpass_a_num[3] = {1, 2, 1};
const float lowpass_a_den[3] = {1, -1.499554496, 0.84821868172};
const float lowpass_b_num[3] = {1, 2, 1};
const float lowpass_b_den[3] = {1, -1.554785180, 0.64929543814};


#endif
