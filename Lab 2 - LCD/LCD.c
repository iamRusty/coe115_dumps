/*
 * File:   ------
 * Author: Rusty
 *
 * Created on March 14, 2017, 1:23 PM
 */

/*
 *    Note: The Nop()s are important for timing
 */

#define FCY 2000000UL
#include "xc.h"
#include <stdlib.h>
#include <libpic30.h>

_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)// & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)

void send4ToLCD(int instruction_4);
void send8ToLCD(int instruction_8);
void send8ToLCD_data(int data_8);
void delay(int delay_constant);
void LCDInit(void);

int main(void) {
    
    // GPIO Initialization
    TRISB = 0x0001;
    AD1PCFG = 0xFFFF;
    
    LCDInit();
    
    send8ToLCD_data(0x52);
    delay(1);
    
    send8ToLCD_data(0x75);
    delay(1);
    
    send8ToLCD_data(0x73);
    delay(1);

    send8ToLCD_data(0x74);
    delay(1);

    send8ToLCD_data(0x79);
    delay(1);    
    
    while(1){
    
    }
    
    return 0;
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
    return;
}

void send8ToLCD_data(int instruction_8){
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
    return;
}

void delay(int delay_constant){
    __delay_ms(delay_constant);
    return;
}


