/*
 * File:   quiz1_lab3.c
 * Author: Rusty
 *
 * Created on March 6, 2017, 2:35 PM
 */

#include "xc.h"
// PIC24FJ64GA002
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)

// PIC24FJ64GB002
//_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
//_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)
        
#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led1_toggle(void);
void led2_toggle(void);
void led3_toggle(void);

int col1_press;
int col2_press;
int col3_press;

int main(void) {
    
    /* Configure ports 
     * RA0 - row 1 (input)
     * RB0 - col 1 (output)
     * RB4 - led 1 (output)
     */
    AD1PCFG = 0xffff;
    TRISA = 0xFFFF;  //All input
    TRISB = 0x0000;  //All output
    LATB = 0xffff;
    
    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     */
    CNPU1 = CNPU1 | 0x0004;
    
    /* Enable interrupts and clear IRQ flag 
     * RA0 - CN2 (CNEN1)
     */
    CNEN1 = CNEN1 | 0x0004;
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    /* Pull down col1 kulto and col3*/
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;

    while(1){
        if (col1_press){
            led1_toggle();
            col1_press = 0; //clear flag
        }
        else if (col2_press){
            led2_toggle();
            col2_press = 0;
        }
        else if (col3_press){
            led3_toggle();
            col3_press = 0;
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
            col1_press = 1; //set flag
            IFS1bits.CNIF = 0;
            return;
        }
        else
            col1_press = 0;

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 1;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            col1_press = 1; //set flag
            IFS1bits.CNIF = 0;
            return;
        }
        else
            col2_press = 0;

        LATBbits.LATB0 = 1;
        LATBbits.LATB1 = 1;
        LATBbits.LATB2 = 0;
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX){
            col1_press = 1; //set flag
            IFS1bits.CNIF = 0;    
            return;
        }
        else
            col3_press = 0; 

    }
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}

void led1_toggle(void){
    LATBbits.LATB4 = !LATBbits.LATB4;
}

void led2_toggle(void){
    LATBbits.LATB5 = !LATBbits.LATB5;
}

void led3_toggle(void){
    LATBbits.LATB7 = !LATBbits.LATB7;
}

