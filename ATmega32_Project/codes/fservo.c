#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


#define FLAME_THRESHOLD 500



// ================= D1 PWM =================
// PB3 OC0

void pwm_d1_init()
{

    DDRB |= (1<<PB3);


    TCCR0 |= (1<<WGM00)|(1<<WGM01);
    TCCR0 |= (1<<COM01);
    TCCR0 |= (1<<CS01)|(1<<CS00);


    OCR0 = 0;

}



// ================= D2 PWM =================
// PD7 OC2

void pwm_d2_init()
{

    DDRD |= (1<<PD7);


    TCCR2 |= (1<<WGM20)|(1<<WGM21);
    TCCR2 |= (1<<COM21);
    TCCR2 |= (1<<CS22);


    OCR2 = 0;

}



void set_right_speed(uint8_t speed)
{
    OCR0 = speed;
}



void set_left_speed(uint8_t speed)
{
    OCR2 = speed;
}




// ================= SERVO =================
// PD4 OC1B


void servo_init()
{

    DDRD |= (1<<PD4);



    // Timer1 Mode 14

    TCCR1A = (1<<WGM11)|(1<<COM1B1);


    TCCR1B = (1<<WGM13)|
             (1<<WGM12)|
             (1<<CS11);



    // 50Hz

    // 8MHz/8 = 1MHz

    // 20000us period

    ICR1 = 20000;


}



void servo_write(uint16_t pulse)
{

    OCR1B = pulse;

}




void servo_sweep()
{

    static uint16_t pos=1000;

    static int8_t dir=1;


    pos += dir*5;



    if(pos>=2000)
        dir=-1;


    if(pos<=1000)
        dir=1;


    servo_write(pos);

}




// ================= ADC =================


void adc_init()
{

    ADMUX = (1<<REFS0);


    ADCSRA =
    (1<<ADEN)|
    (1<<ADPS2)|
    (1<<ADPS1);

}



uint16_t read_adc()
{

    ADMUX &= 0xF0;


    ADCSRA |= (1<<ADSC);


    while(ADCSRA&(1<<ADSC));


    return ADC;

}




// ================= MOTOR =================


void motor_init()
{

    // D1 direction

    DDRB |= (1<<PB0);
    DDRB |= (1<<PB1);
    DDRB |= (1<<PB2);
    DDRB |= (1<<PB4);



    // D2 direction

    DDRD |= (1<<PD0);
    DDRD |= (1<<PD1);
    DDRD |= (1<<PD2);
    DDRD |= (1<<PD3);

}



void forward()
{

    // Right side

    PORTB |= (1<<PB0);
    PORTB &= ~(1<<PB1);


    PORTB |= (1<<PB2);
    PORTB &= ~(1<<PB4);



    // Left side

    PORTD |= (1<<PD0);
    PORTD &= ~(1<<PD1);


    PORTD |= (1<<PD2);
    PORTD &= ~(1<<PD3);

}




void stop_motor()
{

    OCR0=0;
    OCR2=0;

}





// ================= MAIN =================


int main()
{

    motor_init();

    pwm_d1_init();

    pwm_d2_init();

    servo_init();

    adc_init();



    while(1)
    {


        uint16_t flame=read_adc();



        if(flame < FLAME_THRESHOLD)
        {


            stop_motor();


            // freeze servo

            while(read_adc()<FLAME_THRESHOLD)
            {
                _delay_ms(50);
            }


        }


        else
        {


            forward();


            set_right_speed(180);

            set_left_speed(180);



            servo_sweep();


            _delay_ms(20);


        }


    }

}