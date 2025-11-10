#include "stm32l4xx.h"
#include "delayus.h"

void systickDelayUS(int delay){
	// The unit of delay is ms. So the idea is that we should we make a for loop, that counts the number times COUNTFLAG is set to 1. Each cycle of the loop counts exactly 1ms.
	// Repeat till we reach the delay desired. 

	// Reload value is the number of clock cycles per milisec. 
	// Recall that if each period has N processor clock cycles, RELOAD value should be N-1
	SysTick->LOAD = CYCLES_PER_US - 1 ;

	// Clear systick current value register
	SysTick->VAL = 0;

	// Enable systick and select internal clk src
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for(int i = 0; i < delay; i++){
		// This empty loop waits until countflag is set
		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}
	}

	// Finished delaying. Writing 0 to CTRL registers means disabling the timer
	SysTick->CTRL = 0;
}
