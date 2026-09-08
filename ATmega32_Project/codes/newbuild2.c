#define F_CPU 16000000UL

#include <avr/io.h>


void forward()
{
    /*
      LEFT SIDE
      Front Left + Back Left

      Reverse direction because motors are mounted opposite
    */

    // Front Left
    PORTB &= ~(1<<PB0);
    PORTB |=  (1<<PB1);

    // Back Left
    PORTD |=  (1<<PD0);
    PORTD &= ~(1<<PD1);



    /*
      RIGHT SIDE
      Front Right + Back Right
    */

    // Front Right
    PORTB |=  (1<<PB2);
    PORTB &= ~(1<<PB3);

    // Back Right
    PORTD &= ~(1<<PD2);
    PORTD |=  (1<<PD3);
}



void backward()
{
    /*
       Reverse everything
    */


    // Left side

    PORTB |=  (1<<PB0);
    PORTB &= ~(1<<PB1);

    PORTD |=  (1<<PD0);
    PORTD &= ~(1<<PD1);



    // Right side

    PORTB &= ~(1<<PB2);
    PORTB |=  (1<<PB3);

    PORTD &= ~(1<<PD2);
    PORTD |=  (1<<PD3);
}



void stop_car()
{
    PORTB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3));

    PORTD &= ~((1<<PD0)|(1<<PD1)|(1<<PD2)|(1<<PD3));
}



void turn_left()
{
    /*
      Left wheels stop
      Right wheels forward
    */


    // Left motors stop

    PORTB &= ~((1<<PB0)|(1<<PB1));
    PORTD &= ~((1<<PD0)|(1<<PD1));


    // Right motors forward

    PORTB |=  (1<<PB2);
    PORTB &= ~(1<<PB3);

    PORTD |=  (1<<PD2);
    PORTD &= ~(1<<PD3);
}



void turn_right()
{
    /*
      Right wheels stop
      Left wheels forward
    */


    // Right motors stop

    PORTB &= ~((1<<PB2)|(1<<PB3));
    PORTD &= ~((1<<PD2)|(1<<PD3));


    // Left motors forward

    PORTB &= ~(1<<PB0);
    PORTB |=  (1<<PB1);

    PORTD &= ~(1<<PD0);
    PORTD |=  (1<<PD1);
}



int main(void)
{

    // Driver 1
    DDRB |= (1<<PB0) |
            (1<<PB1) |
            (1<<PB2) |
            (1<<PB3);


    // Driver 2
    DDRD |= (1<<PD0) |
            (1<<PD1) |
            (1<<PD2) |
            (1<<PD3);



    while(1)
    {

        forward();

        // keep moving forward

    }


    return 0;
}