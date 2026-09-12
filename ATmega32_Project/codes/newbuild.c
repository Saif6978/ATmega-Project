#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


// ---------------- PWM ----------------

void pwm_init()
{
    // PB3 = OC0
    DDRB |= (1 << PB3);

    // Fast PWM
    // Non-inverting
    // Prescaler 64

    TCCR0 |= (1 << WGM00) | (1 << WGM01);
    TCCR0 |= (1 << COM01);
    TCCR0 |= (1 << CS01) | (1 << CS00);

    OCR0 = 0;
}


// ---------------- Motor Pins ----------------

void motor_init()
{
    DDRB |= (1 << PB0);   // IN1
    DDRB |= (1 << PB1);   // IN2
    DDRB |= (1 << PB2);   // IN3
    DDRB |= (1 << PB4);   // IN4
}


// ---------------- Direction ----------------

void motor_forward()
{
    // Front motor
    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);

    // Back motor
    PORTB |= (1 << PB2);
    PORTB &= ~(1 << PB4);
}


void motor_backward()
{
    // Front motor
    PORTB &= ~(1 << PB0);
    PORTB |= (1 << PB1);

    // Back motor
    PORTB &= ~(1 << PB2);
    PORTB |= (1 << PB4);
}


void motor_stop()
{
    PORTB &= ~(1 << PB0);
    PORTB &= ~(1 << PB1);

    PORTB &= ~(1 << PB2);
    PORTB &= ~(1 << PB4);

    OCR0 = 0;
}


// ---------------- Speed Control ----------------

void set_speed(unsigned char speed)
{
    OCR0 = speed;
}


// ---------------- Acceleration ----------------

void accelerate_forward()
{
    unsigned int speed;


    motor_forward();


    // Increase speed
    for(speed = 40; speed <= 220; speed += 5)
    {
        set_speed(speed);
        _delay_ms(100);
    }


    // Maintain speed
    _delay_ms(3000);


    // Decrease speed
    for(speed = 220; speed >= 40; speed -= 5)
    {
        set_speed(speed);
        _delay_ms(100);
    }
}


void accelerate_backward()
{
    unsigned int speed;


    motor_backward();


    // Increase reverse speed
    for(speed = 90; speed <= 220; speed += 5)
    {
        set_speed(speed);
        _delay_ms(100);
    }


    // Maintain speed
    _delay_ms(3000);


    // Decrease speed
    for(speed = 220; speed >= 90; speed -= 5)
    {
        set_speed(speed);
        _delay_ms(100);
    }
}



// ---------------- Main ----------------

int main()
{
    motor_init();
    pwm_init();


    while(1)
    {

        // Forward acceleration
        accelerate_forward();


        motor_stop();
        _delay_ms(2000);



        // Reverse acceleration
        accelerate_backward();


        motor_stop();
        _delay_ms(3000);

    }

}