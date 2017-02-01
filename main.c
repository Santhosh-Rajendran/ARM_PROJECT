
/*=========================================== INCLUDE FILES =====================================================*/
#include <stdint.h>
#include <stdbool.h> //for boolean operations
#include "Vardef.h"
#include STDLIB_HW_MEMAP_INC
#include STDLIB_HW_TYPES_INC
#include DRIVERLIB_SYSCTL_INC
#include DRIVERLIB_GPIO_INC
#include DRIVERLIB_ADC_INC
#include DRIVERLIB_DEBUG_INC
#include DRIVERLIB_INTERRUPT_INC
#include DRIVERLIB_UART_INC
#include DRIVERLIB_PINMAP_INC
#include DRIVERLIB_TIMER_INC
#define TARGET_IS_BLIZZARD_RB1
#include "inc/tm4c123gh6pm.h"
#include "Funtions/Functions.h"
#include "GPIO/GPIO.h"
#include "ADC/Adc.h"
#include "UART/UART.h"
/*============================================ MACRO DEFINITION ===================================================*/

#define MS2_COUNT_VAL 80000
#define ON 1
#define OFF 0

/*============================================ GLOBAL VARIBLE DECLARATION ========================================*/
uint8_t mtimer = 0;
uint8_t mtimer1 = 0;
uint32_t ADC_Count[4] = {0};
volatile uint32_t AvgCount = 0;
volatile uint32_t TempC = 0;
volatile uint32_t TempF = 0;
volatile bool conversionstatus = 0;
bool _20_MS_EVENT = 0;
bool _40_MS_EVENT = 0;

/*============================================= FUNCTION DEFINITION ================================================*/
static void TimerConfiguration(void);
static void __20ms_function(void);

/*============================================== APPLICATION CODE ==================================================*/
void main(void)
{
	SystemModuleDefinitions();
	GPIOConfigurations();
	ADCSeqConfigure();
	UARTModuleConfig();
	TimerConfiguration();
	
	IntMasterEnable();																  //global interrupt enable
	TimerEnable(TIMER0_BASE, TIMER_A);												  //enable the timer (i.e) start the timer

	while(1)
	{
		if(_20_MS_EVENT == ON)
		{
			_20_MS_EVENT = OFF;
			ADCRoutine();

		}
		if(_40_MS_EVENT == ON)
		{
			_40_MS_EVENT = OFF;
		}
	}
	
}

static void TimerConfiguration(void)
{
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);								  //Configure the timer to be periodic ie continuous
    TimerLoadSet(TIMER0_BASE, TIMER_A, MS2_COUNT_VAL);							      //Load the time period value at which the timeout should occur
	IntEnable(INT_TIMER0A);														      //Enable the timer0 interrupts
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);								  //Enable the timer0 interrupt for timeout condition
}

void Timer0IntHandler(void)
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    __20ms_function();
}

static void __20ms_function(void)
{
    mtimer++;
    mtimer1++;
    
    if(mtimer >= 10)
    {
      _20_MS_EVENT = ON;
      mtimer = 0;
      ADCRoutine();
    }
    
    if(mtimer1 >= 20)
    {
      _40_MS_EVENT = ON;
      mtimer1 = 0; 
    }
}
