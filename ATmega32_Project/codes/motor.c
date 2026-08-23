#define F_CPU 16000000UL

#include <avr/io.h>

int main(void)
{
    // PB0 -> L298N IN1
    // PB1 -> L298N IN2

    DDRB |= (1 << PB0) | (1 << PB1);

    while(1)
    {
        // Motor forward
        PORTB |= (1 << PB0);       // IN1 = HIGH
        PORTB &= ~(1 << PB1);      // IN2 = LOW
    }

    return 0;
}