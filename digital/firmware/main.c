/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 *
 * main.c
 *
 */

#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "filter.h"

#define PI 3.14159265f

//const bool DEBUG = true;

static volatile float currentADC = 0;
//static volatile float currentInput;

static volatile float sine[50];


int main(void)
{

    MAP_WDT_A_holdTimer(); // Turn off watchdog

    P6DIR |= BIT0; // P6.0 set as output.

    // VREF = 1.2V P5.6
    P5SEL0 |= BIT6 | BIT7;
    P5SEL1 |= BIT6 | BIT7;

    REFCTL0 |= REFON;
    REFCTL0 |= REFOUT;
    REFCTL0 &= ~(REFVSEL_3);
    REFCTL0 |= REFVSEL_0;
    while (REFCTL0 & REFGENBUSY);

    //P5.5 ADC A0
    P5SEL1 |= BIT5;
    P5SEL0 |= BIT5;

    // Output pins
    P2DIR = 0xFF;
    P2OUT = 0x00;


    // Configuring pins for high frequency crystal (HFXT) crystal for 48 MHz clock
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); // P4.3 -> 48MHz
    CS_setExternalClockSourceFrequency(32000,48000000);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    CS_startHFXT(false);  // Initialise the HFXT.

    // Initializing the master clock (MCLK) to HFXT (effectively 48MHz)
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Configure ADC14 - pin 5.5
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0);
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A0, false);
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
    MAP_ADC14_enableConversion();
    MAP_ADC14_enableInterrupt(ADC_INT0);
    MAP_Interrupt_enableInterrupt(INT_ADC14);

    // Configure SysTick -> 50kHz
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(960); // 48 MHz / 960 = 50 kHz
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();

    // Enabling MASTER interrupts
    MAP_Interrupt_enableMaster();

    // Enable FPU
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();
    FPU_setFlushToZeroMode(FPU_FLUSH_TO_ZERO_EN);

    // Generate 7kHz sine wave at 50kHz sampling rate
    int N = 0;
    for(N=0;N<50;N++){
        float a = N*7.0f;
        float b = a/50.0f;
        float c = b*2.0f*PI;
        //float t = N*2*PI*7/50;
        float n = sin(c);
        sine[N] = n;
    }

    while (1)
    {
      //MAP_PCM_gotoLPM0();
    }

}

int M = 0;

// 50kHz Systick
void systick_isr(void)
{
	P6OUT |= BIT0; // P6.0 high

  MAP_ADC14_toggleConversionTrigger(); // ADC Conversion

  // 0 magnitude value - (600/1200)*16384
  float input = currentADC;
  float input2 = input - 8192.0f;

  // Bandstop filter input, remove 8kHz tone
  float input_bandstop = bandstop(input2);

  // Multiply by 7kHz sine wave
  float input_bandstop_sine = (input_bandstop * sine[M]);

  // Filter out upper sideband
  filtered = lowpass(input_bandstop_sine);

  float filtered = input_bandstop_sine + 8192.0f;
  float output = (filtered/64.0f);

  P2OUT = output;

  M = M+1;
  if(M > 49){  // 7 full sine waves, 50 samples
    M = 0;
  }

	P6OUT &= ~BIT0; // P6.0 low
}


// ADC Interrupt Handler - ADC_MEM0
void adc_isr(void)
{

	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(status);

  if (status & ADC_INT0){
    currentADC = MAP_ADC14_getResult(ADC_MEM0);
    //currentInput = (currentADC * 1.2) / 16384;
  }

}
