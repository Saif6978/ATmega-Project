#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


void servo_init(void)
{
    // PD4 = OC1B output
    DDRD |= (1 << PD4);

    // Fast PWM Mode 14
    // TOP = ICR1
    // Non-inverting PWM on OC1B

    TCCR1A = (1 << WGM11) | (1 << COM1B1);

    TCCR1B = (1 << WGM13) |
             (1 << WGM12) |
             (1 << CS10);      // Prescaler = 1

    // 20ms period (50Hz)
    ICR1 = 19999;
}


void servo_write(uint16_t pulse)
{
    OCR1B = pulse;
}


int main(void)
{
    servo_init();

    uint16_t pos;


    while(1)
    {

        // Move from 0 degree to 180 degree
        for(pos = 1000; pos <= 2000; pos += 5)
        {
            servo_write(pos);
            _delay_ms(15);
        }


        // Move from 180 degree back to 0 degree
        for(pos = 2000; pos >= 1000; pos -= 5)
        {
            servo_write(pos);
            _delay_ms(15);
        }

    }
}