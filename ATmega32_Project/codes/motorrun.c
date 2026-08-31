#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


void pwm_init()
{
    // PD5 (OC1A) and PD4 (OC1B) output
    DDRD |= (1<<PD5) | (1<<PD4);

    // Fast PWM Mode 14
    // TOP = ICR1

    TCCR1A |= (1<<WGM11);

    TCCR1B |= (1<<WGM13) | (1<<WGM12);

    // Non inverted PWM
    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<COM1B1);

    // Prescaler 8
    TCCR1B |= (1<<CS11);


    // PWM frequency
    // 16MHz/8 = 2MHz
    // 40000 counts = 20ms
    ICR1 = 39999;
}


void motor_pins_init()
{
    // IN1 IN2 IN3 IN4 outputs
    DDRB |= (1<<PB0) |
            (1<<PB1) |
            (1<<PB2) |
            (1<<PB3);
}


// speed: -100 to +100

void left_motor(int speed)
{
    if(speed > 0)
    {
        PORTB |= (1<<PB0);
        PORTB &= ~(1<<PB1);
    }
    else if(speed < 0)
    {
        PORTB &= ~(1<<PB0);
        PORTB |= (1<<PB1);

        speed = -speed;
    }
    else
    {
        PORTB &= ~(1<<PB0);
        PORTB &= ~(1<<PB1);
    }


    OCR1A = (speed * 39999) / 100;
}



void right_motor(int speed)
{
    if(speed > 0)
    {
        PORTB |= (1<<PB2);
        PORTB &= ~(1<<PB3);
    }
    else if(speed < 0)
    {
        PORTB &= ~(1<<PB2);
        PORTB |= (1<<PB3);

        speed = -speed;
    }
    else
    {
        PORTB &= ~(1<<PB2);
        PORTB &= ~(1<<PB3);
    }


    OCR1B = (speed * 39999) / 100;
}



void drive(int left, int right)
{
    left_motor(left);
    right_motor(right);
}



int main(void)
{
    motor_pins_init();
    pwm_init();


    while(1)
    {

        // Straight
        drive(100,100);
        _delay_ms(3000);


        // Left turn
        // left stopped, right full speed
        drive(0,100);
        _delay_ms(2000);


        // Right turn
        drive(100,0);
        _delay_ms(2000);


        // Rotate left
        drive(-100,100);
        _delay_ms(2000);


        // Stop
        drive(0,0);
        _delay_ms(2000);

    }

}