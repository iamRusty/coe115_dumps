/*
 * File:   quiz1_lab3.c
 * Author: Rusty
 *
 * Created on March 6, 2017, 2:35 PM
 */

#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)
        
#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led1_turn_on(void);
void led2_turn_on(void);
void led3_turn_on(void);
void led4_turn_on(void);

int led1_value;
int led2_value;
int led3_value;
int led4_value;

int main(void) {
    
    AD1PCFG = 0xffff;
    TRISA = 0xFFFF;  //All input
    TRISB = 0x0000;  //All output
    LATB = 0xffff;
    
    CNPU1 = CNPU1 | 0x0004;
    CNEN1 = CNEN1 | 0x0004;
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;

    while(1){
        if (led1_value){
            led1_turn_on();
        }
        if (led2_value){
            led2_turn_on();
        }
        if (led3_value){
            led3_turn_on();
        }
        if (led4_value){
            led4_turn_on();
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

void led1_turn_on(void){
    LATBbits.LATB4 = !LATBbits.LATB4;
}

void led2_turn_on(void){
    LATBbits.LATB5 = !LATBbits.LATB5;
}

void led3_turn_on(void){
    LATBbits.LATB7 = !LATBbits.LATB7;
}

void led3_turn_on(void){
    LATBbits.LATB8 = !LATBbits.LATB8L
}

