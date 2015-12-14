/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 */

#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


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


    /* Configuring pins for high frequency crystal (HFXT) crystal for 48 MHz clock */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); // P4.3 -> 48MHz
    CS_setExternalClockSourceFrequency(32000,48000000);
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    CS_startHFXT(false);  // Initialise the HFXT.

    /* Initializing the master clock (MCLK) to HFXT (effectively 48MHz) */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Configure SysTick -> 50kHz
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(960); // 48 MHz / 960 = 50 kHz
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();

     /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    while (1)
    {

    }

}

// 50kHz Systick
void systick_isr(void)
{
	P6OUT |= BIT0; // P6.0 high

	P6OUT &= ~BIT0; // P6.0 low
}


// ADC Interrupt Handler - ADC_MEM0
void adc_isr(void)
{

	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(status);

}
