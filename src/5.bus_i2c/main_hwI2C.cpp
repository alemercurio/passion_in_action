
#include <cstdio>
#include "miosix.h"
#include "drivers/stm32f2_f4_i2c.h"

using namespace std;
using namespace miosix;

// SDA -> I2C Serial DAta line
// SCL -> I2C Serial CLock line

typedef Gpio<GPIOB_BASE,7> sda;
typedef Gpio<GPIOB_BASE,8> scl;

int main()
{
    // Always configure the GPIOs!
    // In this case the mode is "alternate open drain"
    sda::mode(Mode::ALTERNATE_OD);
    scl::mode(Mode::ALTERNATE_OD);
    sda::alternateFunction(4);
    scl::alternateFunction(4);

    // The driver class for harwdare I2C is of singleton type, which means that
    // only an instance is allowed.
    // This method returns a reference to the singleton object.
    // In this case there is no need to play with clock gating, since it is done
    // inside the driver class.
    I2C1Driver& i2c = I2C1Driver::instance();

    while(1)
    {
        // Read temperature from LM75 reading its internal registers.
        // The temperature value is a signed, 16 bit wide one. We have to read
        // two bytes from the device, then.
        // Device address is 98h, the R/W LSB is set internally by the driver
        unsigned char bytes[2] = {0};
        i2c.recv(0x98, &bytes, 2);

        // The LM75 outputs data most significant byte first, so we have to
        // assembly the correct temperature value with some bitwise operations
        short temperature = (bytes[0] << 8) | bytes[1];

        // Altough the sensor returns 16 bits, the value lies between the 16th
        // and the 8th bit: we have to shift down the temperature value by 7
        // positions to compensate for this.
        temperature = temperature >> 7;

        // Convert to float and divide by two, since the LSB is 0.5°C
        float temp = static_cast< float >(temperature) / 2.0f;

        printf("Temperature is %f\n", temp);

        delayMs(1000);
    }

    return 0;
}
