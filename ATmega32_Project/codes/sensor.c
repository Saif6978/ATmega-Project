#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


#define FLAME_THRESHOLD 500
#define DISTANCE_THRESHOLD 15



// ================= PWM INITIALIZATION =================


// D1 PWM
// PB3 = OC0

void pwm_d1_init()
{
    DDRB |= (1 << PB3);

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



// ================= FLAME SENSOR =================


uint8_t flame_detected()
{
    uint16_t value = read_adc();


    if(value < FLAME_THRESHOLD)
        return 1;


    return 0;
}



// ================= ULTRASONIC SENSOR =================


// TRIG = PC0
// ECHO = PC1

void sonar_init()
{
    DDRC |= (1 << PC0);      // TRIG output

    DDRC &= ~(1 << PC1);     // ECHO input
}



void trigger_sonar()
{

    PORTC &= ~(1 << PC0);

    _delay_us(2);


    PORTC |= (1 << PC0);

    _delay_us(10);


    PORTC &= ~(1 << PC0);

}



uint16_t get_distance()
{

    uint16_t count = 0;


    trigger_sonar();



    while(!(PINC & (1 << PC1)));



    while(PINC & (1 << PC1))
    {

        count++;

        _delay_us(1);


        if(count > 30000)
            break;

    }


    return count / 58;

}



uint8_t object_detected()
{

    uint16_t distance = get_distance();


    if(distance <= DISTANCE_THRESHOLD)
        return 1;


    return 0;

}





// ================= MOTOR INITIALIZATION =================


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

    sonar_init();



    while(1)
    {


        if(flame_detected() || object_detected())
        {

            // stop immediately

            stop_motor();



            // wait until both conditions clear

            while(flame_detected() || object_detected())
            {
                _delay_ms(50);
            }


        }


        else
        {

            forward();


            set_right_speed(180);

            set_left_speed(180);

        }


    }

}