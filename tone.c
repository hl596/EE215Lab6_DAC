#include <math.h>
#include "stm32l475xx.h"
#include "tone.h"
#include "delayus.h"

#define NUM_STEPS (64)
#define MAX_DAC_CODE (0xFFF)

#define PI (3.1415927)

int sine_table[NUM_STEPS];

void sinewave_init(void);

void dac_init() {
//Enable necessary clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Enable GPIOA clock for PA4
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN; //Enable DAC1 clock
//Set PA4 for Analog
	GPIOA->MODER |= GPIO_MODER_MODE4; //Set pin 4 in analog mode (0b11<<8)
//Reset DAC CR, then 1) enable DAC channel (1) and 2) configure trigger
//by making TEN1 = 1 and TSEL1 = 0b111 for software trigger
	DAC->CR &= ~(0xFFFF); //Clear the CR register
	DAC->CR |= DAC_CR_EN1; //DAC Channel 1 enable
	DAC->CR |= DAC_CR_TEN1; //DAC Channel 1 Trigger enable
	DAC->CR |= DAC_CR_TSEL1; //Setup software trigger (0b111<<3)
//Initialize DAC channel 1 12-bit right-aligned data holding register
	DAC->DHR12R1 = 0;
} //end dac_init

void dac_set(int value) {
	//Set the output value and trigger
	DAC->DHR12R1 = value;
	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1; //Channel 1 trigger
} //end dac_set

void tone_init(void) {
	dac_init();
	sinewave_init();
}

void tone_play(int period_us, int num_cycles, wavetype wave) {
	int sample;
	int step;
	while(num_cycles-- > 0) {
		for (step = 0; step < NUM_STEPS; step++) {
			switch(wave) {
				case SINE:
					sample = sine_table[step];
					break;
				case SQUARE:
					sample = step < NUM_STEPS / 2 ? 0 : MAX_DAC_CODE;
					break;
				case RAMP:
					sample = (step * MAX_DAC_CODE) / NUM_STEPS;
					break;
			}
			dac_set(sample);
			systickDelayUS(period_us);
		}
	}
}

void sinewave_init(void) {
	int n;
	for (n = 0; n < NUM_STEPS; n++) {
		sine_table[n] = (int)((MAX_DAC_CODE) * (1 + sin(n * 2 * PI / NUM_STEPS)) / 2);
	}
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   
