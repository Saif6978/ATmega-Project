#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


// ================= SERVO =================


void servo_init(void)
{

    // PD4 = OC1B

    DDRD |= (1 << PD4);



    // Timer1 Fast PWM Mode 14
    // TOP = ICR1
    // Non inverted PWM


    TCCR1A = (1 << WGM11) |
             (1 << COM1B1);



    TCCR1B = (1 << WGM13) |
             (1 << WGM12) |
             (1 << CS10);



    // 20ms period
    // F_CPU = 1MHz
    // Prescaler 1

    ICR1 = 19999;


}



void servo_write(uint16_t pulse)
{

    OCR1B = pulse;

}



// ================= SERVO SWEEP =================


void servo_sweep()
{

    static uint16_t pos = 1000;

    static int8_t direction = 1;



    pos += direction * 5;



    if(pos >= 2000)
    {
        direction = -1;
    }



    if(pos <= 1000)
    {
        direction = 1;
    }



    servo_write(pos);

}



// ================= MAIN =================


int main(void)
{


    // PD2 receives fire signal

    DDRD &= ~(1 << PD2);



    servo_init();



    while(1)
    {


        /*
           ATmega1 PD6:

           LOW  = no fire
           HIGH = fire detected

        */


        if(PIND & (1 << PD2))
        {

            // Fire detected

            // Keep servo at current position

            // Do nothing


        }

        else
        {


            // No fire

            // Search movement

            servo_sweep();


        }



        _delay_ms(15);


    }


}