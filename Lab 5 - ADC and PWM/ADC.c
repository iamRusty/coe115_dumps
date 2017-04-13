/*
 * File:   lab5.c
 * Author: Rusty
 *
 * Created on March 8, 2017, 12:23 PM
 */


#include "xc.h"
#include <math.h>
// PIC24FJ64GA002
//_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
//_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)

// PIC24FJ64GB002
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)

int adcvalue;

void Setup(void){
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));    // Release IOLOCK
    RPOR3bits.RP7R = 0x12;                          
    __builtin_write_OSCCONL(OSCCON|(1<<6));         // Engage IOLOCK
}

void ADC_init(){
    //setup ADC configuration bits and TRISB
    TRISB = 0x8000;
    AD1PCFG = 0xFDFF;
    AD1CON1 = 0x20E4;
    AD1CON2 = 0x0;
    AD1CON3 = 0x201;
    AD1CHS = 0x9;
    AD1CSSL = 0x0;    
}

int main(void) {
    ADC_init();
    IEC0bits.AD1IE = 1;    
    //clear interrupt flag
    IFS0bits.AD1IF = 0;   
    
    AD1CON1bits.ADON = 1; //turn on ADC
    
    
    OC1CON1 = 0;
    OC1R = 0;
    OC1RS = 10;
    OC1CON2 = 0x020C;
    OC1CON1 = 0X2008;
    OC1CON1bits.OCM = 0b111;
    PR2 = 79;
    T2CONbits.TON = 0b1;
    
    Setup();

    while(1){
        OC1RS = floor(adcvalue/14.3)+3;
    }
    return 0;
}
void __attribute__ ((interrupt, no_auto_psv)) _ADC1Interrupt(void){
    //Disable interrupt
    IEC0bits.AD1IE = 0; 
    //Clear flag
    IFS0bits.AD1IF = 0;  
    adcvalue = ADC1BUF0;
    //Enable interrupt
    IEC0bits.AD1IE = 1; 
    //Clear flag
    IFS0bits.AD1IF = 0;  
}