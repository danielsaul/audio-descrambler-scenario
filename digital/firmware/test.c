#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "filter.h"

#define PI 3.14159265

int main(void){

      double fs = 50000;
      double f = 8000;

      double t;
      double s;
      double x;

      FILE *j = fopen("file.txt", "w");

      //for(t = 0; t < 1/f; t = t + (1/fs)){
      int i;
      for(i = 0; i < 10000; i++){
          t = i/fs;
          s = sin(2*PI*t*f)*sin(2*PI*t*3000);
          printf("%f , ", s);
          fprintf(j, "%f ,", s);
          x = lowpass(s);
          printf("%f\n", x);
          fprintf(j, "%f\n", x);
      }

      /* i;
      for(i = 0; i < 1000; i++){
          t = i/fs;
          s = 1;
          printf("%f , ", s);
          fprintf(j, "%f ,", s);
          x = lowpass(s);
          printf("%f\n", x);
          fprintf(j, "%f\n", x);
      }*/




fclose(j);


}
