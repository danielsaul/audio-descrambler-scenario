/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 *
 * filter.c
 *
 */

#include "filter.h"

double bandstop_buf[3] = {0,0,0};
double lowpass_a_buf[3] = {0,0,0};
double lowpass_b_buf[3] = {0,0,0};

//////
// Implements Infinite Impulse Response Filter
//////

// Bandstop filter, 2nd order (1 section)
double bandstop(double input){

  update_buffer(bandstop_buf);
  double output = filter(input, bandstop_buf, bandstop_num, bandstop_den);

  return output;

}

// Lowpass filter, 4th order (2 sections)
double lowpass(double input){

  update_buffer(lowpass_a_buf);
  double outa = filter(input, lowpass_a_buf, lowpass_a_num, lowpass_a_den);
  update_buffer(lowpass_b_buf);
  double output = filter(outa, lowpass_b_buf, lowpass_b_num, lowpass_b_den);

  return output;

}

// Second-order IIR filter, direct form II
double filter(double x, double *w, double *b, double *a){

  w[2] = x - (a[1] * w[1]) - (a[2] * w[0]);
  double y = (b[0] * w[2]) + (b[1] * w[1]) + (b[2] * w[0]);

  return y;

}

// Move buffer around
// n -> n-1
// n-1 -> n-2
void update_buffer(double *w){

  w[0] = w[1];
  w[1] = w[2];

}
