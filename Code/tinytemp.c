#include "tinyOLED.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#define MYDELAY 500

const char temp[] PROGMEM = "TEMP:";
const char tooHot[] PROGMEM = "TOO HOT!";
const char blank[] PROGMEM = "        ";
const char degC[] PROGMEM = " C";
const char degF[] PROGMEM = " F";
const char decimal[] PROGMEM = ".";

const unsigned int maxTemp = 22;

#define NUMSAMPLES 25  // #ADC Samples to average
#define VREF 1.1        // ADC reference voltage

void adc_init(void){
    ADMUX = 0x83;
    ADCSRA = 0x83;
}

unsigned int get_adc(){
    ADCSRA |= (1<<ADSC);
    while ((ADCSRA & (1<<ADIF))==0);
    return ADCL | (ADCH<<8);
}

void WDT_OFF()
{
    MCUSR &= ~(1 << WDRF);
    WDTCR = (1 << WDCE) | (1 << WDE);
    WDTCR = 0x00;
}

int main(void){

    WDT_OFF();
    unsigned long int totalValue;
    unsigned int digitalValue, tempx10;
    float tempC, tempF;
    
    OLED_init();
    OLED_clear();
    adc_init();
    DDRB = 1<<DDB4;
    
    //DDRB = 1<<PB3;

    char buffer[3];
    
    while(1){

        //OLED_clear();
        totalValue = 0;
        for (int i = 0; i < NUMSAMPLES; i++)
            totalValue += get_adc(); // Get a sample from temp sensor
        digitalValue = totalValue / NUMSAMPLES;
        
        tempC = digitalValue * 1.0 * VREF / 10.24 - 50.;
        // PCB Calibration
        tempC = tempC - 6.;

        tempF = tempC * 9. / 5. + 32.;

        // Print in C 
        tempx10 = tempC * 10; 
        OLED_cursor(1,0);
        OLED_printP(temp);
        itoa((int)(tempx10/10),buffer,10);
        unsigned int i;
        for (i = 0; i < strlen(buffer); i++){
            OLED_printC(buffer[i]);
        }
        OLED_printP(decimal);
        itoa((int)(tempx10%10),buffer,10);
        for (i = 0; i < strlen(buffer); i++){
            OLED_printC(buffer[i]);
        }
        OLED_printP(degC);

        // Print in F 
        tempx10 = tempF * 10; 
        OLED_cursor(1,1);
        OLED_printP(temp);
        itoa((int)(tempx10/10),buffer,10);
        for (i = 0; i < strlen(buffer); i++){
            OLED_printC(buffer[i]);
        }
        OLED_printP(decimal);
        itoa((int)(tempx10%10),buffer,10);
        for (i = 0; i < strlen(buffer); i++){
            OLED_printC(buffer[i]);
        }
        OLED_printP(degF);


        // Print too hot warning
        if (tempC > maxTemp){
            OLED_cursor(1,3);
            OLED_printP(tooHot);
        }else{
            OLED_cursor(1,3);
            OLED_printP(blank);
        }

        PORTB ^= 1<<PB4;

        wdt_enable(WDTO_1S);
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
        while(1);
    }
    return 0;
}

