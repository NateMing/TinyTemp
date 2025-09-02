/******************************************************************
* megaTemp.c Firmware for a digital thermometer based on ATmega328P,
* TMP-36 sensor and SSD1306 OLED display. Displays TMP-36 sensor voltage (in mV)
* Temp (in F or C depending on switch position). Features a Too-Hot warning
* and LED when temp exceeds a pre-set value (deg F)
* Version 1.0 D. McLaughlin Initial release for ECE-304 2/28/24
**************************************************************/

#include "tinyTemp.h"
#define NUMSAMPLES 25  // #ADC Samples to average
#define VREF 1.1        // ADC reference voltage
#define MAXTEMP 70      // Too-Hot LED turns on at this temp (Deg F)
#define TOO_HOT_LED PD2 // LED indicator pin
#define F_C_SELECT PD3  // F vs C temp switch. C when pressed.

int main(void)
{
    float tempC, tempF, analogVoltage;
    //unsigned char switch_state, too_hot;
    unsigned int digitalValue, tempx10;
    unsigned long int totalValue;
    float x = 2.2;
    char numBuffer[3];
    char decimalBuffer[3];
    adc_init();
    OLED_init();
    OLED_clear();
    

    while (1)
    {
        // Acquire & average NUMSAMPLES temp measurements
        totalValue = 0;
        for (int i = 0; i < NUMSAMPLES; i++)
            totalValue += get_adc(); // Get a sample from temp sensor
        digitalValue = totalValue / NUMSAMPLES;
        
        // Convert to temp & analog voltage in mV
        tempC = digitalValue * 1.0 * VREF / 10.24 - 50.; 
        tempF = tempC * 9. / 5. + 32.;
        itoa((int)(x), buffer, 10);
        x++;
        unsigned char i;
        OLED_cursor(10, 1);    // set cursor position
        for (i = 0; i < strlen(buffer); i++){
            OLED_printC(buffer[i]);
        }
        //analogVoltage = digitalValue * 1.0 * VREF / 1024; 
        //analogVoltage = analogVoltage * 1000;  

        //switch_state = (PIND & (1 << F_C_SELECT));
        //too_hot = (tempF > MAXTEMP);
        /*
        if (switch_state) // Choose the correct scale and mult by 10
            tempx10 = tempF * 10.;
        else
            tempx10 = tempC * 10.;
        
        OLED_GoToLine(1);
        OLED_DisplayNumber(10, analogVoltage, 4);
        OLED_DisplayString("mV");
        OLED_GoToLine(3);
        OLED_DisplayNumber(10, tempx10 / 10, 2);
        OLED_DisplayString(".");
        OLED_DisplayNumber(10, tempx10 % 10, 1);
        if (switch_state)
            OLED_DisplayString(" Deg F");
        else
            OLED_DisplayString(" Deg C");
        
        if (too_hot){
            PORTD |= (1 << TOO_HOT_LED); // MAXTEMP LED ON
            OLED_GoToLine(5);
            OLED_DisplayString("Too Hot!");
        } else {
            PORTD &= ~(1 << TOO_HOT_LED); // MAXTEMP LED OFF
            OLED_GoToLine(5);
            OLED_DisplayString("        ");
        }
        */
        _delay_ms(500);
    }
    return 0;
}

// Initialize ADC peripheral
void adc_init(void)
{
    ADMUX = 0b00000011; 
    if (F_CPU > 1000000)
        ADCSRA = 0x87; // Enable ADC; divide by 128 for 16 MHz clock
    else
        ADCSRA = 0x83; // Enable ADC; divide by 8 for 1 MHZ clock
}

// Read ADC value
unsigned int get_adc()
{
    ADCSRA |= (1 << ADSC); // Start ADC conversion
    while ((ADCSRA & (1 << ADIF)) == 0)
        ;                       // Wait till ADC finishes
    ADCSRA |= (1<<ADIF);        // Clear ADIF flag
    return ADCL | (ADCH << 8);  // Read ADCL first !
}