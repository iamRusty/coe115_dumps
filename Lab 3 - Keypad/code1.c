/*
 * File:   main.c
 * Author: Snap
 *
 * Created on February 20, 2017, 8:22 PM
 */


#include "xc.h"

// PIC24FJ64GA002
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)

// PIC24FJ64GB002
//_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
//_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)

int main(void) {
    
    /* Configure ports 
     * RA0 - row 1 (input)
     * RB0 - col 1 (output)
     * RB4 - led 1 (output)
     */
    AD1PCFG = 0xffff;
    TRISA = 0x0000;
    TRISB = 0xffee;
    LATB = 0xffff;
    
    /* Enable internal pullups
     * RA0 - CN2 (CNPU1)
     */
    CNPU1 = CNPU1 | 0x0004;
    
    /* Pull down col1 */
    LATBbits.LATB0 = 0;
    while(1){
        if (!PORTAbits.RA0) //button pressed
            LATBbits.LATB4 = 0b0; //led 1 on
        else
            LATBbits.LATB4 = 0b1;
    }
    
    return 0;
}
