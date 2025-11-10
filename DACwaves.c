#include "stm32l4xx.h"
#include "tone.h"

#define BUTTON_PORT GPIOC
#define BUTTON_PIN (1U << 13)

void button_init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    GPIOC->MODER &= ~(3U << 26); 
    GPIOC->PUPDR &= ~(3U << 26);
    GPIOC->PUPDR |= (1U << 26);
}

int main(void) {

    button_init();
    tone_init();

    while (1) {
        // Check if the button is pressed
        if ((GPIOC->IDR & (1U << 13)) == 0) {
            // Wait a short time for debounce
            for (int i = 0; i < 50000; i++); 

            // Play the wave when the button is detected as pressed
            tone_play(30, 200, SINE);

            // Delay to prevent re-triggering while the button is held
            while ((GPIOC->IDR & BUTTON_PIN) == 0) {
            }
        }
    }
}