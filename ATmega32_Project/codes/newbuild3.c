#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


void forward()
{
    // Front Left
    PORTB &= ~(1<<PB0);
    PORTB |=  (1<<PB1);

    // Front Right
    PORTB |=  (1<<PB2);
    PORTB &= ~(1<<PB3);


    // Back Left (reversed motor mounting)
    PORTD |=  (1<<PD0);
    PORTD &= ~(1<<PD1);

    // Back Right (reversed motor mounting)
    PORTD &= ~(1<<PD2);
    PORTD |=  (1<<PD3);
}



void backward()
{
    // Front Left
    PORTB |=  (1<<PB0);
    PORTB &= ~(1<<PB1);

    // Front Right
    PORTB &= ~(1<<PB2);
    PORTB |=  (1<<PB3);


    // Back Left
    PORTD &= ~(1<<PD0);
    PORTD |=  (1<<PD1);

    // Back Right
    PORTD |=  (1<<PD2);
    PORTD &= ~(1<<PD3);
}



void stop_car()
{
    PORTB &= ~((1<<PB0)|(1<<PB1)|(1<<PB2)|(1<<PB3));

    PORTD &= ~((1<<PD0)|(1<<PD1)|(1<<PD2)|(1<<PD3));
}


void turn_left()
{
    // LEFT SIDE STOP
    // Front Left
    PORTB &= ~((1<<PB0) | (1<<PB1));

    // Back Left
    //PORTD &= ~((1<<PD0) | (1<<PD1));
    PORTD &= ~(1<<PD1);
    PORTD |= (1<<PD0);

    // RIGHT SIDE FORWARD

    // Front Right
    PORTB |=  (1<<PB2);
    PORTB &= ~(1<<PB3);


    // Back Right
    //PORTD &= ~(1<<PD2);
    //PORTD |=  (1<<PD3);
    PORTD &= ~((1<<PD2) | (1<<PD3));
}

void turn_right()
{
    // RIGHT SIDE STOP

    // Front Right
    PORTB &= ~((1<<PB2) | (1<<PB3));

    // Back Right
    //PORTD &= ~((1<<PD2) | (1<<PD3));
    PORTD |= (1<<PD3);
    PORTD &= ~(1<<PD2);

    // LEFT SIDE FORWARD

    // Front Left
    PORTB &= ~(1<<PB0);
    PORTB |=  (1<<PB1);


    // Back Left
    //PORTD |=  (1<<PD0);
    //PORTD &= ~(1<<PD1);
    PORTD &= ~((1<<PD0) | (1<<PD1));
}



int main(void)
{

    // Driver 1
    // PB0-PB3 = front motors

    DDRB |= (1<<PB0) |
            (1<<PB1) |
            (1<<PB2) |
            (1<<PB3);



    // Driver 2
    // PD0-PD3 = back motors

    DDRD |= (1<<PD0) |
            (1<<PD1) |
            (1<<PD2) |
            (1<<PD3);



    while(1)
    {

        // Move forward
        forward();
        _delay_ms(150);


        // Stop
        stop_car();
        _delay_ms(200);


        // Move backward
        backward();
        _delay_ms(200);


        // Stop
        stop_car();
        _delay_ms(100);



        // Rotate left
        //rotate_left();
        turn_left();
        _delay_ms(50);


        // Stop
        stop_car();
        _delay_ms(100);



        // Rotate right
        turn_right();
        _delay_ms(50);


        // Stop
        stop_car();
        _delay_ms(100);

    }


    return 0;
}