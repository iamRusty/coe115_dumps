/*
 * File:   ------
 * Author: Rusty
 *
 * Created on February 20, 2017, 8:22 PM
 * Based off Sir Snap's original codes
 */


#include "xc.h"
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)// & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)
        
#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);

void led1_toggle(void);
void led2_toggle(void);
void led3_toggle(void);
void led4_toggle(void);

int row1_press;
int row2_press;
int row3_press;
int row4_press;

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
     * RA0 - CN2 (CNPU1) bit 2
       RA1 - CN2 (CNPU1) bit 3
       RA2 - CN2 (CNPU2) bit 14
       RA3 - CN2 (CNPU2) bit 13
     */
    CNPU1 = CNPU1 | 0x000C; // Bit 2 and 3
    CNPU2 = CNPU2 | 0x6000; // Bit 13 and 14
    
    /* Enable interrupts and clear IRQ flag 
     * RA0 - CN2 (CNEN1) bit 2
       RA1 - CN2 (CNEN1) bit 3
       RA2 - CN2 (CNEN2) bit 14
       RA3 - CN2 (CNEN2) bit 13
     */

    CNEN1 = CNEN1 | 0x000C;
    CNEN2 = CNEN2 | 0x6000; 
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    /* Pull down col1 */
    LATBbits.LATB0 = 0;
    while(1){
        if (row1_press){
            led1_toggle();
            row1_press = 0; //clear flag
        }
        else if (row2_press){
            led2_toggle();
            row2_press = 0; //clear flag
        }
        else if (row3_press){
            led3_toggle();
            row3_press = 0; //clear flag
        }
        else if (row4_press){
            led4_toggle();
            row4_press = 0; //clear flag
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

    else if (!PORTAbits.RA1){
        /* Software debounce */
        while ((!PORTAbits.RA1) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row2_press = 1; //set flag
        else
            row2_press = 0;
    }

    else if (!PORTAbits.RA2){
        /* Software debounce */
        while ((!PORTAbits.RA2) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row3_press = 1; //set flag
        else
            row3_press = 0;
    }

    else if (!PORTAbits.RA3){
        /* Software debounce */
        while ((!PORTAbits.RA3) && (deb_ctr < DEB_MAX)){
            deb_ctr++;
        }
        if (deb_ctr == DEB_MAX)
            row4_press = 1; //set flag
        else
            row4_press = 0;
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

void led4_toggle(void){
    LATBbits.LATB8 = !LATBbits.LATB8;
}