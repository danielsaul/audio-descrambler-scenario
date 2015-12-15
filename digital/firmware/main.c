/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 */

#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

const bool DEBUG = true;

static volatile uint_fast16_t buffer[8];


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
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
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

    while (1)
    {
      //MAP_PCM_gotoLPM0();
    }

}

// 50kHz Systick
void systick_isr(void)
{
	P6OUT |= BIT0; // P6.0 high

  MAP_ADC14_toggleConversionTrigger(); // ADC Conversion

	P6OUT &= ~BIT0; // P6.0 low
}


// ADC Interrupt Handler - ADC_MEM0
void adc_isr(void)
{

	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(status);

  if (status & ADC_)

}
