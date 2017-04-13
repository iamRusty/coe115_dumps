/*
 * File:   lab5.c
 * Author: Rusty
 *
 * Created on March 8, 2017, 12:23 PM
 */


#include "xc.h"
// PIC24FJ64GA002
//_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
//_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)

// PIC24FJ64GB002
_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV)
_CONFIG3 (SOSCSEL_IO)


int main(void)  
  TRISB = 0;
  OC1CON1 = 0;                  //Clear OC1CON1
  OC1R = 0;                     //set value of OC1R
  OC1RS = 56;                   //set value of OC1RS
  OC1CON2 = 0x020C;             //Set value of OC1CON2
  OC1CON1 = 0x2008;             //set initial value of OC1CON1 with OC module off
  OC1CON1bits.OCM = 0b111;      //set values of OCM bits
  PR2 = 79;                     //set value of PR2
  T2CONbits.TON = 0b1;          //Initiate Timer 2

  
  __builtin_write_OSCCONL(OSCCON & (~(1<<6)));
  RPOR3bits.RP7R = 0x12;
  __builtin_write_OSCCONL(OSCCON|(1<<6));
  

  while(1){    
  
  }
    return 0;
}
