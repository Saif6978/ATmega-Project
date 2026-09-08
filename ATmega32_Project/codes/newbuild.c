#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


// ---------------- DRIVER 1 ----------------
// Front wheels
// PB0 PB1 = Front Left
// PB2 PB3 = Front Right


// ---------------- DRIVER 2 ----------------
// Back wheels
// PD0 PD1 = Back Left
// PD2 PD3 = Back Right



void stop_all()
{
    PORTB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3));

    PORTD &= ~((1<<PD0)|(1<<PD1)|(1<<PD2)|(1<<PD3));
}



void forward()
{
    // Front Left
    PORTB |= (1<<PB0);
    PORTB &= ~(1<<PB1);

    // Front Right
    PORTB |= (1<<PB2);
    PORTB &= ~(1<<PB3);


    // Back Left
    PORTD |= (1<<PD0);
    PORTD &= ~(1<<PD1);

    // Back Right
    PORTD |= (1<<PD2);
    PORTD &= ~(1<<PD3);
}



void backward()
{
    // Front Left
    PORTB &= ~(1<<PB0);
    PORTB |= (1<<PB1);

    // Front Right
    PORTB &= ~(1<<PB2);
    PORTB |= (1<<PB3);


    // Back Left
    PORTD &= ~(1<<PD0);
    PORTD |= (1<<PD1);

    // Back Right
    PORTD &= ~(1<<PD2);
    PORTD |= (1<<PD3);
}



void turn_left()
{
    // Left wheels stopped

    // Front Left
    PORTB &= ~(1<<PB0);
    PORTB &= ~(1<<PB1);

    // Back Left
    PORTD &= ~(1<<PD0);
    PORTD &= ~(1<<PD1);



    // Right wheels forward

    // Front Right
    PORTB |= (1<<PB2);
    PORTB &= ~(1<<PB3);


    // Back Right
    PORTD |= (1<<PD2);
    PORTD &= ~(1<<PD3);
}



void turn_right()
{
    // Right wheels stopped

    // Front Right
    PORTB &= ~(1<<PB2);
    PORTB &= ~(1<<PB3);

    // Back Right
    PORTD &= ~(1<<PD2);
    PORTD &= ~(1<<PD3);



    // Left wheels forward

    // Front Left
    PORTB |= (1<<PB0);
    PORTB &= ~(1<<PB1);


    // Back Left
    PORTD |= (1<<PD0);
    PORTD &= ~(1<<PD1);
}



int main(void)
{

    // PB0-PB3 output
    DDRB |= (1<<PB0) |
            (1<<PB1) |
            (1<<PB2) |
            (1<<PB3);


    // PD0-PD3 output
    DDRD |= (1<<PD0) |
            (1<<PD1) |
            (1<<PD2) |
            (1<<PD3);



    while(1)
    {

        forward();
        _delay_ms(3000);


        stop_all();
        _delay_ms(1000);


        backward();
        _delay_ms(3000);


        stop_all();
        _delay_ms(1000);


        turn_left();
        _delay_ms(2000);


        stop_all();
        _delay_ms(1000);


        turn_right();
        _delay_ms(2000);


        stop_all();
        _delay_ms(2000);

    }

}