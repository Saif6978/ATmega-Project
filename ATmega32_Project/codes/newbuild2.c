#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


// ================= PWM INITIALIZATION =================


// D1 PWM
// PB3 = OC0

void pwm_d1_init()
{
    DDRB |= (1 << PB3);

    // Fast PWM
    // Non-inverting
    // Prescaler 64

    TCCR0 |= (1 << WGM00) | (1 << WGM01);
    TCCR0 |= (1 << COM01);
    TCCR0 |= (1 << CS01) | (1 << CS00);

    OCR0 = 0;
}


// D2 PWM
// PD5 = OC1A

void pwm_d2_init()
{
    DDRD |= (1 << PD5);

    // Timer1 Fast PWM 8-bit
    // Non-inverting OC1A
    // Prescaler 64

    TCCR1A |= (1 << COM1A1);
    TCCR1A |= (1 << WGM10);

    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS11) | (1 << CS10);

    OCR1A = 0;
}



void set_right_speed(uint8_t speed)
{
    OCR0 = speed;
}


void set_left_speed(uint8_t speed)
{
    OCR1A = speed;
}



// ================= PIN INITIALIZATION =================

void motor_init()
{

    // D1 direction pins

    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB4);



    // D2 direction pins

    DDRD |= (1 << PD0);
    DDRD |= (1 << PD1);
    DDRD |= (1 << PD2);
    DDRD |= (1 << PD3);

}



// ================= MOVEMENT FUNCTIONS =================


// Both sides forward

void forward()
{

    // D1 Right side

    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);

    PORTB |= (1 << PB2);
    PORTB &= ~(1 << PB4);



    // D2 Left side

    PORTD |= (1 << PD0);
    PORTD &= ~(1 << PD1);

    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);

}




// Both sides backward

void backward()
{

    // D1 Right side

    PORTB &= ~(1 << PB0);
    PORTB |= (1 << PB1);

    PORTB &= ~(1 << PB2);
    PORTB |= (1 << PB4);



    // D2 Left side

    PORTD &= ~(1 << PD0);
    PORTD |= (1 << PD1);

    PORTD &= ~(1 << PD2);
    PORTD |= (1 << PD3);

}




void stop_motor()
{

    OCR0 = 0;
    OCR1A = 0;


}



// ================= TURNING =================


// Left turn:
// Left wheels slow
// Right wheels fast

void turn_left()
{
    forward();

    // Left wheels slower
    // Right wheels faster

    set_left_speed(80);
    set_right_speed(240);
}


void turn_right()
{
    forward();

    // Left wheels faster
    // Right wheels slower

    set_left_speed(240);
    set_right_speed(80);
}



// ================= MAIN =================


int main()
{

    motor_init();

    pwm_d1_init();
    pwm_d2_init();



    while(1)
    {


        // -------- Forward --------

        forward();

        set_left_speed(220);
        set_right_speed(180);

        _delay_ms(300);



        // -------- Stop --------

        stop_motor();

        _delay_ms(500);




        // -------- Backward --------

       // backward();

        //set_left_speed(180);
        //set_right_speed(180);

        //_delay_ms(4000);




        // -------- Stop --------

        //stop_motor();

        //_delay_ms(2000);




        // -------- Turn Left --------

        // turn_left();

        // _delay_ms(300);



        // stop_motor();

        // _delay_ms(100);




        // -------- Turn Right --------

        // turn_right();

        // _delay_ms(300);



        // stop_motor();

        // _delay_ms(1000);


    }
}