
#include <cstdio>
#include <stdint.h>
#include "miosix.h"

using namespace std;
using namespace miosix;

// We will use the led connected to GPIO line PD12
typedef Gpio<GPIOD_BASE, 12> led;

int main()
{
    // Configure GPIO as alternate function in order to connect it to timer
    // output
    led::mode(Mode::ALTERNATE);

    // Timer 4 is on alternate function 2
    led::alternateFunction(2);

    // Enable the timer peripheral by setting to 1 the corresponding bit into
    // the clock gating register. This gives clock to peripheral's hardware
    // allowing to read and write hardware. This operation must be done before
    // accessing any of the peripheral's registers, otherwise reading and/or
    // writing to them will have no effect.
    {
        FastInterruptDisableLock dLock;
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
        RCC_SYNC();
    }

    // Now we can configure the timer in order to generate a pwm signal on
    // compare channel 1
    //
    // The timer has a clock divider configured through the prescaler register;
    // the timer's counter is incremented by one with a tick frequency given by
    // the following equation: F_tick = F_bus / (PSC + 1), where F_bus is the
    // frequency of the peripheral bus. In this case F_bus = 84MHz.
    //
    // This peripheral has also an auto-reload register which specifies the
    // maximum value the internal counter can reach. The counter goes from zero
    // to the value of ARR register and then back to zero.
    // The frequency of the pwm signal depends also on the value of the ARR
    // register: F_pwm = F_tick / ARR.
    // Putting together the two equation we have that the pwm frequency is given
    // by: F_pwm = F_bus / ((PSC + 1) x ARR).

    TIM4->CNT = 0;                      // Clear internal counter

    TIM4->PSC = 1;                      // Division by 2, F_tick = F_bus/2 = 42MHz

    TIM4->ARR = 32768;                  // F_pwm = F_tick / 32768 = 1281Hz

    TIM4->CCR1 = 0;                     // Clear compare register for channel 1

    TIM4->EGR = TIM_EGR_UG;             // Generate and update event to load
                                        // data into peripheral's registers

    TIM4->CCMR1 |= TIM_CCMR1_OC1M_1     // Configure compare channel 1 to work
                | TIM_CCMR1_OC1M_2      // in PWM mode 1: output is high as long
                                        // as CNT < CCR1

                | TIM_CCMR1_OC1PE;      // Enable preload

    TIM4->CCER  |= TIM_CCER_CC1E;       // Enable compare channe 1

    TIM4->CR1   |= TIM_CR1_CEN;         // Enable timer

    while(1)
    {
        unsigned char pwmPercent = 0;

        scanf("%d", &pwmPercent);       // Pwm value as user input, range 0 - 100

        if(pwmPercent > 100)
        {
            pwmPercent = 100;
        }

        // Computation of value to be put in compare register 1. Since the maximum
        // counter value is given by the ARR register, the compare register value
        // is a percentage of this value.
        // We need to use a 32bit variable in order to have enough digits to 
        // represent 32768 * 100 without loss of precision
        uint32_t regValue = (TIM4->ARR * pwmPercent) / 100;
        TIM4->CCR1 = regValue;

        Thread::sleep(100);
    }

    return 0;
}
