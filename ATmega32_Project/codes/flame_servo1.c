#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


#define FLAME_THRESHOLD 500



// ================= PWM =================


// D1 PB3 OC0

void pwm_d1_init()
{

    DDRB |= (1<<PB3);

    TCCR0 |= (1<<WGM00)|(1<<WGM01);
    TCCR0 |= (1<<COM01);
    TCCR0 |= (1<<CS01)|(1<<CS00);

    OCR0=0;

}



// D2 PD5 OC1A

void pwm_d2_init()
{

    DDRD |= (1<<PD5);


    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<WGM10);

    TCCR1B |= (1<<WGM12);
    TCCR1B |= (1<<CS11)|(1<<CS10);


    OCR1A=0;

}



void set_right_speed(uint8_t speed)
{
    OCR0=speed;
}


void set_left_speed(uint8_t speed)
{
    OCR1A=speed;
}



// ================= ADC =================


void adc_init()
{

    ADMUX=(1<<REFS0);


    ADCSRA=(1<<ADEN)|
           (1<<ADPS2)|
           (1<<ADPS1);

}



uint16_t read_adc()
{

    ADMUX &=0xF0;


    ADCSRA|=(1<<ADSC);


    while(ADCSRA&(1<<ADSC));


    return ADC;

}



// ================= MOTOR =================


void motor_init()
{

    // D1

    DDRB|=(1<<PB0);
    DDRB|=(1<<PB1);
    DDRB|=(1<<PB2);
    DDRB|=(1<<PB4);



    // D2

    DDRD|=(1<<PD0);
    DDRD|=(1<<PD1);
    DDRD|=(1<<PD2);
    DDRD|=(1<<PD3);

}



void forward()
{

    // Right side

    PORTB|=(1<<PB0);
    PORTB&=~(1<<PB1);


    PORTB|=(1<<PB2);
    PORTB&=~(1<<PB4);



    // Left side

    PORTD|=(1<<PD0);
    PORTD&=~(1<<PD1);


    PORTD|=(1<<PD2);
    PORTD&=~(1<<PD3);

}



void stop_motor()
{

    OCR0=0;
    OCR1A=0;

}




// ================= MAIN =================


int main()
{

    motor_init();

    pwm_d1_init();

    pwm_d2_init();

    adc_init();



    // Fire communication pin

    DDRD |= (1<<PD6);



    while(1)
    {


        uint16_t flame=read_adc();



        if(flame < FLAME_THRESHOLD)
        {


            // stop car

            stop_motor();


            // send fire signal

            PORTD |= (1<<PD6);



        }


        else
        {


            // no fire signal

            PORTD &= ~(1<<PD6);



            forward();


            set_right_speed(100);

            set_left_speed(100);


        }


    }

}