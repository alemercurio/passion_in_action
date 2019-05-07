#include <cstdio>
#include "miosix.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE, 1> channel1;
typedef Gpio<GPIOA_BASE, 2> channel2;

// Read an analog input from a specific adc channel

// The process is divided in steps:
//  1. Select the channel to convert
//  2. Signal the adc to start a conversion
//  3. Wait for the adc to finish the conversion
//  4. Read the result
uint16_t convertChannel(uint8_t channel)
{
    // 1. Select the channel to read
    // The regular sequence registers (SQR1, SQR2, SQR3) are used to specify 
    // the sequence of channels to convert.

    // Since we only need to convert one channel at a time, we only need to 
    // specify the bits for the first channel in the sequence. 
    // That is, we need to set the SQ1 bits in the SQR3 register (bits 0 to 4) 
    // to the number identifying the channel we want to convert

    // To make sure we write only the SQ1 bits, we mask the channel variable 
    // with 0b00011111, imposing all the bits after the 5 least significant 
    // ones to 0.
    ADC1->SQR3 = (uint32_t)(channel & 0x1F); 

    // 2. Start conversion setting the SWSTART bit to 1 in the 
    // Control Register 2 (CR2)
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // 3. Wait for conversion to finish.
    // We wait until the adc sets the EOC (End Of Conversion) bit to 1 in the 
    // status register (SR)
    while ((ADC1->SR & ADC_SR_EOC) == 0)
    {

    }

    // When it's done, clear the status register setting every bit to 0
    ADC1->SR = 0;

    // 4. When the adc has finished the conversion, the result is put in the the 
    // 2 least significant bytes of the Data Register (DR)
    uint16_t result = ADC1->DR & 0x0000FFFF;

    return result;
}

int main()
{
    // Configure GPIOs in INPUT_ANALOG mode in order to read analog data through
    // the ADC.
    channel1::mode(Mode::INPUT_ANALOG);
    channel2::mode(Mode::INPUT_ANALOG);

    // Enable the adc peripheral by setting to 1 the corresponding bit into
    // the clock gating register. This gives clock to peripheral's hardware
    // allowing to read and write hardware. This operation must be done before
    // accessing any of the peripheral's registers, otherwise reading and/or
    // writing to them will have no effect.
    {
        FastInterruptDisableLock dLock;
        RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

        RCC_SYNC();
    }

    // Specify how many channels we want to convert at a time.
    // We want to convert only one channel, so we set the L bits in the SQR1 
    // register (bits 20 to 23) to 0. 

    // All the bits in the SQR1 register must remain to 0 in the case of 
    // one single conversion, so its easier to set the whole register to 0.
    ADC1->SQR1 = 0x0;

    // Power on the ADC setting the ADON bit in the Control Register 2 (CR2)
    ADC1->CR2 |= ADC_CR2_ADON;

    // Convert the channels and display the results every second
    for (;;)
    {
        uint16_t result_ch1 = convertChannel(1); // Convert channel 1
        uint16_t result_ch2 = convertChannel(2); // Convert channel 2

        printf("CH1: %d, CH2: %d\n", result_ch1, result_ch2);

        Thread::sleep(1000);
    }
}