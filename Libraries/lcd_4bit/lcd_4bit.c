/*
 * File:   lcd_4bit.c
 * Author: Rusty
 *
 * Created on March 18, 2017, 2:23 PM
 */

/*
 *    Note: The Nop()s are important for timing
 */


// This will optimally work in 4MHz environment
#define FCY 4000000UL
#include "xc.h"
#include <stdlib.h>
#include <libpic30.h>
#include "lcd_4bit.h"
#include <string.h>


//PIC24GA002
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)// & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)

/*
 _CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)
 */        
        
void send4ToLCD(int instruction_4);
void send8ToLCD(int instruction_8);
void send8ToLCDData(int data_8);
void delay(int delay_constant);
void LCDInit(void);
void writeString(char* word);
void cursorPos(int DDRAM_address);
void clearDisplay(void);
void clearLine1(void);
void clearLine2(void);

// Only supports alpha-numeric
void writeString(char* string){
    int __string_length__;
    __string_length__ = strlen(string);
    int count;
    count = 0;
    
    // The first 128 characters in supported LCD characters
    // are based on ASCII Table
    while(count < __string_length__){
        send8ToLCDData(string[count]);
        count++;
    }
    
    return;
}

void LCDInit(void){    
    // No data should be displayed to or from the display for 15ms
    delay(15);
    
    // Function Set command: (8-bit Interface)
    send4ToLCD(0x0003);
    delay(5);
    
    send4ToLCD(0x0003);
    //__delay_us(100);
    delay(15);
    
    // After this command is written, BF can be checked
    send4ToLCD(0x0003);
    delay(15);
    
    // Function Set: Sets interface to 4-bit
    send8ToLCD(0x0002);
    delay(15);
    
    // Function Set (Interface = 4-bit, 
    //    Set N for number of lines	(1)
    //    Set F for character font    (0)
    send8ToLCD(0x0028);
    delay(15);
    
    // Display Off
    send8ToLCD(0x0008);
    delay(15);
    
    // Clear Display 
    send8ToLCD(0x0001);
    delay(15);
    
    // Entry Mode Set
    send8ToLCD(0x0006);
    delay(15);
    
    // Display ON
    //  Set C for Cursor	(1)
    //  Set B for Blink     (1)
    send8ToLCD(0x000F);
    delay(15);
}

void send4ToLCD(int instruction_4){
    instruction_4 = instruction_4 << 8;
    PORTB = instruction_4;
    Nop();
    PORTB = PORTB | 0x4000;
    Nop();
    PORTB = PORTB & 0x0F00;
    Nop();
    return;
}

void send8ToLCD(int instruction_8){
    int ls_data = instruction_8 & 0x000F;
    ls_data = ls_data << 8;
    int ms_data = instruction_8 & 0x00F0;
    ms_data = ms_data << 4;
    
    PORTB = ms_data;
    Nop();
    PORTB = PORTB | 0x4000;
    Nop();
    PORTB = PORTB & 0x0F00;
    Nop();

    PORTB = ls_data;
    Nop();
    PORTB = PORTB | 0x4000;
    Nop();
    PORTB = PORTB & 0x0F00;
    Nop();
    //delay(1)
    return;
}

void send8ToLCDData(int instruction_8){
    int ls_data = instruction_8 & 0x000F;
    ls_data = ls_data << 8;
    ls_data = ls_data | 0x2000;
    int ms_data = instruction_8 & 0x00F0;
    ms_data = ms_data << 4;
    ms_data = ms_data | 0x2000;
    
    PORTB = ms_data;
    Nop();
    PORTB = PORTB | 0x4000;
    Nop();
    PORTB = PORTB & 0x0F00;
    Nop();

    PORTB = ls_data;
    Nop();
    PORTB = PORTB | 0x4000;
    Nop();
    PORTB = PORTB & 0x0F00;
    Nop();
    delay(1);
    return;
}

void delay(int delay_constant){
    __delay_ms(delay_constant);
    return;
}

void cursorPos(int DDRAM_address){
    send8ToLCD(DDRAM_address);
    delay(1);
}

void clearDisplay(){
    clearLine1();
    clearLine2();
    cursorPos(0x80);
}
void clearLine1(void){
    cursorPos(0x80);
    writeString("                ");
}

void clearLine2(void){
    cursorPos(0xC0);
    writeString("                ");
}