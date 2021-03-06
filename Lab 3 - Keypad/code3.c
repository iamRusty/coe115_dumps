/*
 * File:   -----
 * Author: Rusty
 *
 * Created on February 20, 2017, 8:22 PM
* Based off Sir Snap's original codes
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

int row1_press;

int main(void) {
    
    /* Configure ports 
     * RA0 - row 1 (input)
     * RB0 - col 1 (output)
     * RB4 - led 1 (output)
     */
    AD1PCFG = 0xffff;
    TRISA = 0x0001;
    TRISB = 0xff00;
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
    
    /* Pull down col1 */
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
    while(1){
        if (row1_press){
            led1_toggle();
            row1_press = 0; //clear flag
        }
    }
    
    return 0;
}

void __attribute__((interrupt)) _CNInterrupt(void){
    int deb_ctr = 0; //debounce counter
    
    if (!PORTAbits.RA0){
        /* Software debounce */
        while ((!PORTAbits.RA0) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row1_press = 1; //set flag
        else
            row1_press = 0;
    }
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}

void led1_toggle(void){
    LATBbits.LATB4 = !LATBbits.LATB4;
}
