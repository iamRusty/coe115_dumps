/*
 * File:   -----
 * Author: Rusty
 *
 * Created on March 11, 2017, 2:03 AM
 */

#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)// & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)
        
#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led1_turn_on(int count1);
void led2_turn_on(int count2);
void led3_turn_on(int count3);
void led4_turn_on(int count4);

int led1_value;
int led2_value;
int led3_value;
int led4_value;

int main(void) {
    
    AD1PCFG = 0xffff;
    TRISA = 0xFFFF;  //All input
    TRISB = 0x0000;  //All output
    LATB = 0xffff;
    
    CNPU1 = CNPU1 | 0x000C; // Bit 2 and 3
    CNPU2 = CNPU2 | 0x6000; // Bit 13 and 14
    
    CNEN1 = CNEN1 | 0x000C;
    CNEN2 = CNEN2 | 0x6000; 
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;

    while(1){
        // Reset Pulldown
        IEC1bits.CNIE = 0; 
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 0;
        IEC1bits.CNIE = 1; 
        if (led1_value){
            led1_turn_on(0);
        }
        else {
            led1_turn_on(1);
        }
        if (led2_value){
            led2_turn_on(0);
        }
        else {
            led2_turn_on(1);
        }
        if (led3_value){
            led3_turn_on(0);
        }
        else {
            led3_turn_on(1);
        }
        if (led4_value){
            led4_turn_on(0);
        }
        else {
            led4_turn_on(1);
        }
    }
    
    return 0;
}

void __attribute__((interrupt)) _CNInterrupt(void){
    int deb_ctr = 0; //debounce counter  
    
    if (!PORTAbits.RA0){
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 1;
            led2_value = 0;
            led3_value = 0;
            led4_value = 0;
            IFS1bits.CNIF = 0;
            return;
        }
            
        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 0;
            led2_value = 1;
            led3_value = 0;
            led4_value = 0;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 1;
            led2_value = 1;
            led3_value = 0;
            led4_value = 0;
            IFS1bits.CNIF = 0;   
            return;
        }
    }
    else if (!PORTAbits.RA1){
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 0;
            led2_value = 0;
            led3_value = 1;
            led4_value = 0;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 1;
            led2_value = 0;
            led3_value = 1;
            led4_value = 0;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 0;
            led2_value = 1;
            led3_value = 1;
            led4_value = 0;
            IFS1bits.CNIF = 0;    
            return;
        }
    }
    else if (!PORTAbits.RA2){
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 1;
            led2_value = 1;
            led3_value = 1;
            led4_value = 0;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 0;
            led2_value = 0;
            led3_value = 0;
            led4_value = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 1;
            led2_value = 0;
            led3_value = 0;
            led4_value = 1;
            IFS1bits.CNIF = 0;  
            return;
        }
    }
    else if (!PORTAbits.RA3){
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 0;
            led2_value = 1;
            led3_value = 0;
            led4_value = 1;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 0;
            led2_value = 0;
            led3_value = 0;
            led4_value = 0;
            IFS1bits.CNIF = 0;
            return;
        }

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            led1_value = 1;
            led2_value = 1;
            led3_value = 0;
            led4_value = 1;
            IFS1bits.CNIF = 0; 
            return;
        }
    }       
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}


// If 0 -> LED On (Negative logic)
void led1_turn_on(int count1){
    LATBbits.LATB4 = count1;
}

void led2_turn_on(int count2){
    LATBbits.LATB5 = count2;
}

void led3_turn_on(int count3){
    LATBbits.LATB7 = count3;
}

void led4_turn_on(int count4){
    LATBbits.LATB8 = count4;
}

void reset_pulldown(void){
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
}