
#include <cstdio>
#include "miosix.h"

using namespace std;
using namespace miosix;

// Create an easy-to-use label for the GPIO connected to PORT A, line 5 (also
// indicated with PA5).
// In this case it corresponds with the on-board green led.

typedef Gpio<GPIOA_BASE, 5> led;

// An alternative form of declaration, more C++-ish can be the following:
// using led = Gpio<GPIOA_BASE, 5>;

int main()
{
    // Set the GPIO line in output mode
    led::mode(Mode::OUTPUT);

    while(1)
    {
        // Put the GPIO line in high state (turn on the led)
        led::high();

        // Wait for 500 milliseconds. One can also use the function delayMs()
        Thread::sleep(500);

        // Put the GPIO line in low state (turn off the led)
        led::low();

        Thread::sleep(500);
    }

    return 0;
}
