#ifndef DELAYUS_H
#define DELAYUS_H

#include "stm32l4xx.h"

#define CYCLES_PER_US 16
#define CTRL_ENABLE (1U<<0)
#define CTRL_CLKSRC (1U<<2)
#define CTRL_COUNTFLAG (1u<<16)

void systickDelayUS(int delay);

#endif