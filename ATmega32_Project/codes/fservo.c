#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define FLAME_THRESHOLD 500



// ================= GLOBAL SERVO =================


volatile uint16_t servo_position = 1500;
volatile uint16_t servo_tick = 0;

volatile uint8_t servo_high = 0;



// ================= PWM D1 =================
// PB3 OC0

void pwm_d1_init()
{
    DDRB |= (1 << PB3);


    TCCR0 |= (1 << WGM00) | (1 << WGM01);
    TCCR0 |= (1 << COM01);
    TCCR0 |= (1 << CS01) | (1 << CS00);


    OCR0 = 0;
}



// ================= PWM D2 =================
// PD5 OC1A

void pwm_d2_init()
{
    DDRD |= (1 << PD5);


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




// ================= SERVO TIMER2 =================


void servo_init()
{

    DDRD |= (1 << PD4);


    // Timer2 normal mode

    TCCR2 = 0;


    // Prescaler 64

    TCCR2 |= (1 << CS22);



    // Enable overflow interrupt

    TIMSK |= (1 << TOIE2);



    TCNT2 = 0;


    sei();

}



// Timer2 overflow interrupt

ISR(TIMER2_OVF_vect)
{

    static uint16_t count = 0;


    TCNT2 = 6;



    count++;



    /*
       Timer2 tick:

       8MHz / 64 = 125kHz

       one tick = 8us

    */


    if(servo_high)
    {

        if(count >= (servo_position / 8))
        {

            PORTD &= ~(1 << PD4);

            servo_high = 0;

            count = 0;
        }

    }

    else
    {

        if(count >= 2500)
        {

            PORTD |= (1 << PD4);

            servo_high = 1;

            count = 0;
        }

    }

}



void servo_angle(uint16_t pulse)
{
    servo_position = pulse;
}



void servo_sweep()
{

    static uint16_t pos = 1000;

    static int8_t direction = 1;



    pos += direction * 10;



    if(pos >= 2000)
    {
        direction = -1;
    }


    if(pos <= 1000)
    {
        direction = 1;
    }



    servo_angle(pos);

}





// ================= ADC =================


void adc_init()
{

    ADMUX |= (1 << REFS0);


    ADCSRA |= (1 << ADEN);


    ADCSRA |= (1 << ADPS2) | (1 << ADPS1);

}



uint16_t read_adc()
{

    ADMUX &= 0xF0;


    ADCSRA |= (1 << ADSC);



    while(ADCSRA & (1 << ADSC));


    return ADC;

}





// ================= MOTOR INIT =================


void motor_init()
{

    // D1

    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB4);



    // D2

    DDRD |= (1 << PD0);
    DDRD |= (1 << PD1);
    DDRD |= (1 << PD2);
    DDRD |= (1 << PD3);

}





// ================= MOTOR CONTROL =================


void forward()
{

    // Right side

    PORTB |= (1 << PB0);
    PORTB &= ~(1 << PB1);


    PORTB |= (1 << PB2);
    PORTB &= ~(1 << PB4);



    // Left side

    PORTD |= (1 << PD0);
    PORTD &= ~(1 << PD1);


    PORTD |= (1 << PD2);
    PORTD &= ~(1 << PD3);

}



void stop_motor()
{

    OCR0 = 0;

    OCR1A = 0;

}





// ================= MAIN =================


int main()
{

    motor_init();


    pwm_d1_init();

    pwm_d2_init();


    adc_init();


    servo_init();



    while(1)
    {


        uint16_t flame = read_adc();



        if(flame < FLAME_THRESHOLD)
        {

            stop_motor();


            // Servo freezes here


            while(read_adc() < FLAME_THRESHOLD)
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