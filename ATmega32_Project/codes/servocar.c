#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


// Servo signal: PD4 (OC1B)

void servo_init()
{
    DDRD |= (1 << PD4);


    /*
       Timer1 Fast PWM Mode 14

       TOP = ICR1

       Frequency:
       F_CPU = 8MHz
       Prescaler = 8

       Timer frequency = 1MHz

       ICR1 = 20000

       Period = 20ms (50Hz)
    */


    TCCR1A |= (1 << COM1B1);

    TCCR1A |= (1 << WGM11);


    TCCR1B |= (1 << WGM13);
    TCCR1B |= (1 << WGM12);


    TCCR1B |= (1 << CS11);



    ICR1 = 20000;

}



void servo_angle(uint16_t pulse)
{
    OCR1B = pulse;
}



int main()
{

    servo_init();



    while(1)
    {

        // 0 degree
        servo_angle(1000);
        _delay_ms(1000);



        // 90 degree
        servo_angle(1500);
        _delay_ms(1000);



        // 180 degree
        servo_angle(2000);
        _delay_ms(1000);



        // 90 degree
        servo_angle(1500);
        _delay_ms(1000);

    }

}