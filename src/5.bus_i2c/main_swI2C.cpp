
#include <cstdio>
#include "miosix.h"
#include "util/software_i2c.h"

using namespace std;
using namespace miosix;

// SDA -> I2C Serial DAta line
// SCL -> I2C Serial CLock line

typedef Gpio<GPIOB_BASE,7> sda;
typedef Gpio<GPIOB_BASE,8> scl;

int main()
{

    // Here we use software I2C driver, a class that emulates the behaviour of
    // a master I2C device entirely in software.
    // There is no need to configure the GPIOs because this is done internally
    // and there is no need to use clock gating, since no hardware peripheral is
    // involved
    typedef SoftwareI2C<sda, scl> i2c;
    i2c::init();

    while(1)
    {
        // Read temperature from LM75 reading its internal registers.
        // The temperature value is a signed, 16 bit wide one. We have to read
        // two bytes from the device, then.
        // Device address is 98h, we have to set to 1 the LSB to signal that we
        // want to read data
        unsigned char bytes[2] = {0};
        i2c::sendStart();               // Start transaction
        i2c::send(0x98 | 0x01);         // Slave address, read
        bytes[0] = i2c::recvWithAck();  // Read first byte, send ack
        bytes[1] = i2c::recvWithNack(); // Read second byte, send nack
        i2c::sendStop();                // End transaction

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
