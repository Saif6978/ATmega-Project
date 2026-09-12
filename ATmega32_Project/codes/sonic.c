#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>


#define DISTANCE_THRESHOLD 15   // cm



// ================= SONAR =================

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



    // Wait for echo HIGH

    while(!(PINC & (1 << PC1)));



    // Measure echo HIGH duration

    while(PINC & (1 << PC1))
    {

        count++;

        _delay_us(1);


        if(count > 30000)
            break;

    }



    // Convert time to cm

    return count / 58;
}



// ================= MAIN =================

int main()
{

    sonar_init();


    // PA1 as output for LED

    DDRA |= (1 << PA1);



    while(1)
    {

        uint16_t distance = get_distance();



        if(distance <= DISTANCE_THRESHOLD)
        {

            // Object detected

            PORTA |= (1 << PA1);

        }

        else
        {

            // No object

            PORTA &= ~(1 << PA1);

        }



        _delay_ms(100);

    }

}