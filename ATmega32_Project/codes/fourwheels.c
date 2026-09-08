#define F_CPU 16000000UL

#include <avr/io.h>

int main(void)
{
    // Driver 1: PB0-PB3 output
    DDRB |= (1<<PB0) |
            (1<<PB1) |
            (1<<PB2) |
            (1<<PB3);


    // Driver 2: PD0-PD3 output
    DDRD |= (1<<PD0) |
            (1<<PD1) |
            (1<<PD2) |
            (1<<PD3);


    while(1)
    {
        /*
          Driver 1 forward

          IN1 = 1
          IN2 = 0
          IN3 = 1
          IN4 = 0
        */

        PORTB |= (1<<PB0);
        PORTB &= ~(1<<PB1);

        PORTB |= (1<<PB2);
        PORTB &= ~(1<<PB3);


        /*
          Driver 2 forward

          IN1 = 1
          IN2 = 0
          IN3 = 1
          IN4 = 0
        */

        PORTD |= (1<<PD0);
        PORTD &= ~(1<<PD1);

        PORTD |= (1<<PD2);
        PORTD &= ~(1<<PD3);
    }


    return 0;
}