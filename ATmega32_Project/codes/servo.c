#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


void servo_init()
{
    // PD5 (OC1A) as output
    DDRD |= (1 << PD5);


    /*
       Timer1 Fast PWM Mode 14

       TOP = ICR1
    */

    TCCR1A = (1 << COM1A1) |
             (1 << WGM11);


    TCCR1B = (1 << WGM13) |
             (1 << WGM12) |
             (1 << CS11);


    // 50Hz servo frequency
    // 16MHz / 8 = 2MHz
    // 20ms = 40000 counts

    ICR1 = 39999;
}


void servo_angle(uint16_t pulse)
{
    OCR1A = pulse;
}



int main(void)
{
    servo_init();


    while(1)
    {

        // 0 degree
        servo_angle(2000);
        _delay_ms(1000);


        // 90 degree
        servo_angle(3000);
        _delay_ms(1000);


        // 180 degree
        servo_angle(4000);
        _delay_ms(1000);

    }


    return 0;
}