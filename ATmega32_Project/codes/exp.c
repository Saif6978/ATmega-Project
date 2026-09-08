#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
// LCD DATA PORT
#define LCD_PORT PORTD
#define LCD_DDR  DDRD
// LCD CONTROL
#define LCD_CTRL PORTC
#define LCD_CTRL_DDR DDRC
#define RS PC6
#define EN PC7
// ---------------- LCD FUNCTIONS ----------------
void LCD_Enable()
{
    LCD_CTRL |= (1<<EN);
    _delay_us(1);

    LCD_CTRL &= ~(1<<EN);
    _delay_us(100);
}
void LCD_SendNibble(unsigned char data)
{
    LCD_PORT &= 0x0F;

    LCD_PORT |= (data & 0xF0);

    LCD_Enable();
}
void LCD_Command(unsigned char cmd)
{
    LCD_CTRL &= ~(1<<RS);

    LCD_SendNibble(cmd);
    LCD_SendNibble(cmd<<4);

    _delay_ms(2);
}
void LCD_Data(unsigned char data)
{
    LCD_CTRL |= (1<<RS);

    LCD_SendNibble(data);
    LCD_SendNibble(data<<4);

    _delay_ms(2);
}
void LCD_String(char *str)
{
    while(*str)
    {
        LCD_Data(*str);
        str++;
    }
}
void LCD_Init()
{

    LCD_DDR |= 0xF0;

    LCD_CTRL_DDR |= (1<<RS)|(1<<EN);


    _delay_ms(20);
    // 4-bit initialization
    LCD_SendNibble(0x30);
    _delay_ms(5);

    LCD_SendNibble(0x30);
    _delay_us(200);

    LCD_SendNibble(0x30);
    _delay_us(200);
    LCD_SendNibble(0x20);
    LCD_Command(0x28);   // 4-bit, 2 line
    LCD_Command(0x0C);   // Display ON
    LCD_Command(0x06);   // Increment cursor
    LCD_Command(0x01);   // Clear
    _delay_ms(5);
}
// ---------------- ADC FUNCTIONS ----------------
void ADC_Init()
{
    // AVCC reference = 5V
    // ADC0 selected
    ADMUX = (1<<REFS0);
    // Enable ADC
    // Prescaler 128
    ADCSRA =
    (1<<ADEN) |
    (1<<ADPS2) |
    (1<<ADPS1) |
    (1<<ADPS0);
}
uint16_t ADC_Read()
{
    // Select ADC0
    ADMUX &= 0xF0;
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return ADC;
}
// ---------------- MAIN ----------------
int main()
{
    char buffer[16];
    uint16_t adc_value;
    float voltage;
    LCD_Init();
    ADC_Init();
    while(1)
{
    char buffer[16];
    uint16_t adc_value;
    uint16_t voltage;
    adc_value = ADC_Read();
    voltage = (adc_value * 500UL) / 1023;
    LCD_Command(0x01);
    // First line: ADC value
    LCD_String("ADC:");
    sprintf(buffer,"%d", adc_value);
    LCD_String(buffer);
    // Second line: Voltage
    LCD_Command(0xC0);
    LCD_String("V:");
    sprintf(buffer,"%d.%02dV",
            voltage/100,
            voltage%100);
    LCD_String(buffer);
    _delay_ms(500);
}
}