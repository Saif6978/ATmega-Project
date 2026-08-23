#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // Set PB0 as output
    DDRB |= (1 << PB0);

    while (1)
    {
        // Turn LED ON
        PORTB |= (1 << PB0);
        _delay_ms(100);

        // Turn LED OFF
        PORTB &= ~(1 << PB0);
        _delay_ms(100);
    }

    return 0;
}