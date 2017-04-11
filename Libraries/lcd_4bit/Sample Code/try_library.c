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
    lcdInit();
    
    noBlink();
    noCursor();
    lcdPrint("LCD Test");
    setCursor(0xC0);
    lcdPrint("Number Test: ");
    delay(5000);
    lcdIntPrint(115);
    delay(2000);
    clearDisplay();
    lcdPrint("Loading");
    setCursor(0xC0);

    int count;
    count = 0;
    while (count < 16){
        lcdWrite(0xFFFF);
        delay(1000);
        count++;
    }
    
    clearDisplay();
    lcdPrint("Loading Complete");
    delay(2500);
    clearDisplay();
    
    lcdPrint("  Hello World");
    setCursor(0xC0);
    lcdPrint("  I <3 CoE115");
    while(1){
    }
    
    return 0;
}
