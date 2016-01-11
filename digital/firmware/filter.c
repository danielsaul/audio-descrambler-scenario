/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 *
 * filter.c
 *
 */

#include "filter.h"

float bandstop_buf[3] = {0,0,0};
float lowpass_a_buf[3] = {0,0,0};
float lowpass_b_buf[3] = {0,0,0};

//////
// Implements Infinite Impulse Response Filter
//////

// Bandstop filter, 2nd order (1 section)
float bandstop(float input){

  update_buffer(bandstop_buf);
  float output = filter(input, bandstop_buf, bandstop_num, bandstop_den);

  return output; //* bandstop_g;

}

// Lowpass filter, 4th order (2 sections)
float lowpass(float input){

  update_buffer(lowpass_a_buf);
  float outa = filter(input, lowpass_a_buf, lowpass_a_num, lowpass_a_den);
  update_buffer(lowpass_b_buf);
  float output = filter(outa/* * lowpass_a_g*/, lowpass_b_buf, lowpass_b_num, lowpass_b_den);

  return output; //* lowpass_b_g;

}

// Second-order IIR filter, direct form II
float filter(float x, float *w, float *b, float *a){

  w[2] = x - (a[1] * w[1]) - (a[2] * w[0]);
  float y = (b[0] * w[2]) + (b[1] * w[1]) + (b[2] * w[0]);

  return y;

}

// Move buffer around
// n -> n-1
// n-1 -> n-2
void update_buffer(float *w){

  w[0] = w[1];
  w[1] = w[2];

}
