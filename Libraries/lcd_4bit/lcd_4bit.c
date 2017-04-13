/*
 *    Note: The Nop()s are important for timing
 */
/*
    D4 - RB8
    D5 - RB9
    D6 - RB10
    D7 - RB11
*/

#define FCY 4000000UL                   // This will optimally work in 4MHz environment
#define DECIMAL_POINT_PRECISION 3       // Default 3 decimal points to print
#define DELAY_ITERATION 498             // Default delay iteration for 4MHz environment 
#include "xc.h"
#include "stdlib.h"
#include "libpic30.h"
#include "lcd_4bit.h"
#include "string.h"
#include <stdio.h>

static int blink_value;
static int cursor_value;

//PIC24GA002
//_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
//_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)// & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)

/*
 _CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)
 */        

        
void send4ToLCD(int instruction_4);
void send8ToLCD(int instruction_8);
void lcdWrite(int data_8);
void delay(int delay_constant);
void lcdInit(void);
void lcdPrint(char* word);
void lcdIntPrint(int value);
void lcdFloatPrint(float float_number);
void setCursor(int DDRAM_address);
void clearDisplay(void);
void clearLine1(void);
void clearLine2(void);
void noBlink(void);
void noCursor(void);

// Only supports alpha-numeric
void lcdPrint(char* string){
    int __string_length__;
    __string_length__ = strlen(string);
    int count;
    count = 0;
    
    // The first 128 characters in supported LCD characters
    // are based on ASCII Table
    while(count < __string_length__){
        lcdWrite(string[count]);
        count++;
    }
    
    return;
}

void lcdIntPrint(int value){
    char __string_buffer__[16];
    sprintf(__string_buffer__, "%i", value);
    lcdPrint(__string_buffer__);
    return;
}

void lcdFloatPrint(float float_number){
    int abscissa = (int) float_number;
    float decimal_part;
    decimal_part = float_number - abscissa;
    int count = 0;
    while (count < DECIMAL_POINT_PRECISION){
        decimal_part = decimal_part * 10;
        count++;
    }
    lcdIntPrint(abscissa);
    lcdPrint(".");
    lcdIntPrint((int) decimal_part); 
    return;
}

void lcdInit(void){
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
    
    // Initialize default values of blink and cursor
    blink_value = 1;
    cursor_value = 1;
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

void lcdWrite(int instruction_8){
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
    __delay_us(100);
    return;
}

void delay(int delay_constant){
    //__delay_ms(delay_constant);
    int count, count2;
    
    for (count2 = 0; count2 < delay_constant; count2++){
        for (count = 0; count < DELAY_ITERATION; count++){
        Nop();
        Nop();
        }
    }
    return;
}


void setCursor(int DDRAM_address){
    send8ToLCD(DDRAM_address);
    delay(1);
}

void clearDisplay(){
    clearLine1();
    clearLine2();
    setCursor(0x80);
}
void clearLine1(void){
    setCursor(0x80);
    lcdPrint("                ");
    setCursor(0x80);
}

void clearLine2(void){
    setCursor(0xC0);
    lcdPrint("                ");
    setCursor(0xC0);
}

void noBlink(void){
    if (cursor_value)
        send8ToLCD(0x0E);
    else
        send8ToLCD(0x0C);
}

void noCursor(void){
    if (cursor_value)
        send8ToLCD(0x0D);
    else
        send8ToLCD(0x0C);
}