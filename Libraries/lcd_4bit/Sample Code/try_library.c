/*
 * File:   try_library.c
 * Author: Rusty
 *
 * Created on March 18, 2017, 3:35 PM
 */

#define FCY 4000000UL

#include "xc.h"
#include "lcd_4bit.h"
#include <stdio.h>
#include <stdlib.h>

char __string_buffer_[16];

int main(void) {

    TRISB = 0x0001;
    AD1PCFG = 0xFFFF;
    LCDInit();
    
    //send8ToLCD_data(0x52);
    writeString("hello");
    cursorPos(0xC0);
    sprintf(__string_buffer_, "%i", 89);
    writeString(__string_buffer_);
    delay(5000);
    clearDisplay();
    delay(10000);
    writeString("Hello World");
    while(1){
    }
    
    return 0;
}
