#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // PD4 (OC1B) output
    DDRD |= (1 << PD4);

    // Fast PWM Mode 14
    // TOP = ICR1
    TCCR1A = (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12);

    // Prescaler = 8
    TCCR1B |= (1 << CS11);

    // 50Hz PWM
    ICR1 = 39999;

    while(1)
    {
        // Center position (~90 degree)
        OCR1B = 3000;

        _delay_ms(20);
    }

    return 0;
}