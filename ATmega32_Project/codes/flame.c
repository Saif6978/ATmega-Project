#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


void ADC_init()
{
    // Use AVCC as ADC reference
    ADMUX |= (1<<REFS0);


    // Enable ADC
    ADCSRA |= (1<<ADEN);


    // ADC clock = 16MHz/128 = 125kHz
    ADCSRA |= (1<<ADPS2) |
              (1<<ADPS1) |
              (1<<ADPS0);
}



uint16_t ADC_read(uint8_t channel)
{
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | channel;


    // Start conversion
    ADCSRA |= (1<<ADSC);


    // Wait for conversion
    while(ADCSRA & (1<<ADSC));


    return ADC;
}



int main(void)
{

    // PC0 as output for bulb
    DDRC |= (1<<PC0);


    ADC_init();


    uint16_t flame_value;


    while(1)
    {

        // Read flame sensor AO
        flame_value = ADC_read(0);


        /*
          Adjust this value after testing.

          Flame usually gives lower ADC value.
        */

        if(flame_value < 500)
        {
            // Flame detected
            PORTC |= (1<<PC0);
        }
        else
        {
            // No flame
            PORTC &= ~(1<<PC0);
        }


        _delay_ms(100);

    }

}