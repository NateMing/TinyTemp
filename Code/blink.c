#define LED PB0
#define MYDELAY 1000
#include "tinyOLED.h"
#include <avr/io.h>
#include <util/delay.h>

const char Message1[] PROGMEM = "COUNT:";

int main(void)
{
    OLED_init();
    OLED_clear();
    DDRB = 1<<PB4;

    while(1){
        PORTB |= 1<<PB4;    // LED on
        OLED_cursor(10, 1);    // set cursor position
        OLED_printP(Message1); // print message 1
        _delay_ms(MYDELAY);
        PORTB &= ~(1<<PB4); // LED off
        OLED_clear();
        _delay_ms(MYDELAY);
    }
}