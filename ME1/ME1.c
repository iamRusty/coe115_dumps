/*
 * File:   -----
 * Author: Rusty Mina
 *         John Cabuyadao
 *
 * Created on March 11, 2017, 2:03 AM
 */
/*
 v1 
    * functions below are coded but not tested
        * user prompt at beginning
        * display target temp vs current temp
        * asterisk = start, # = pause (not confident on this)
        * automatic regulation (not calibrated, tsambatix only)
    * still missing
        * button 2/8 to increase/decrease target temperature respectively
    
 
 
 
 
 */
#include "xc.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "lcd_4bit.h"


_CONFIG1 (FWDTEN_OFF & JTAGEN_OFF)
_CONFIG2 (POSCMOD_NONE & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRCPLL)// & PLL96MHZ_OFF & PLLDIV_NODIV)
//_CONFIG3 (SOSCSEL_IO)
        
#define DEB_MAX 10

void __attribute__((interrupt)) _CNInterrupt(void);
int result_temp;        // current temperature
char tmp_str[3];
char limit[3];          // targ_temp in str
int adcvalue;
int press=0;            // number of presses
int key;                // value of key pressed
int targ_temp;          // target temp (from usr input)
int start(void);        // asterisk start
int prompt(void);       // user prompt
void template();        // Target temp vs current temp
void PWM_init();
void ADC_init();
void ADC_setup(void);   // PWM's output pin


int main(void) {
    TRISB = 0x8000;
    PWM_init();
    ADC_init();
    lcdInit();
    ADC_setup();
    IFS0bits.AD1IF = 0;   
    
    AD1CON1bits.ADON = 1;       //Initiate ADC
    T2CONbits.TON = 0b1;        //Initiate Timer 2
    AD1PCFG = 0xffff;
    TRISA = 0xFFFF;             //All input
    //TRISB = 0x0000;             //All output
    
    CNPU1 = CNPU1 | 0x000C;     // Bit 2 and 3
    CNPU2 = CNPU2 | 0x6000;     // Bit 13 and 14
    
    CNEN1 = CNEN1 | 0x000C;
    CNEN2 = CNEN2 | 0x6000; 
    IEC1bits.CNIE = 1; 
    IFS1bits.CNIF = 0;
    
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;

    prompt();
    press = 0; //reset press #
    start();
    
    while(1){
        // Reset Pulldown
        IEC1bits.CNIE = 0; 
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 0;
        IEC1bits.CNIE = 1;
        //result_temp = floor(adcvalue/3.03); //resulting temp in deg *display this in LCD 
        //sprintf(tmp_str, "%s", result_temp); //check this
        //lcdPrint(tmp_str); //builds at this pt
        
        template();
        //automatic regulation
        if (result_temp < targ_temp)
        {
            
            if(result_temp >= 0.9*targ_temp)
                OC1RS = OC1RS - 5;
            else
                OC1RS = PR2+2;
        }
        else if (result_temp > 1.1*targ_temp)
        {
            OC1RS = OC1RS - (PR2 - 5);
        }
        template();
        
        //pressing # pauses regulation but displays current temp
        if (press == 1 && key == 10)
        {
            while(key!= 11)
            {
                template();
                OC1RS = 0;
            }
            press = 0;
        }
        //increase or decrease PW manually
        /*
        if (press == 1){
             if (key==2 && OC1RS<PR2-2){
                OC1RS = OC1RS + 2;
                press=0;
            }
            else if(key==8 && OC1RS>2){
                OC1RS = OC1RS - 2;
                press=0;
            }
        }*/
       
    }
    
    return 0;
}

// Keypad Interrupt
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
            key=1;
            press=1;
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
            
            key = 2;
            press=1;
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
            key=3;
            press=1;
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
            key=4;
            press=1;
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
            key=5;
            press=1;
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
            key=6;
            press=1;
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
            key=7;
            press=1;
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
           
            press=1;
            key=8;
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
            key=9;
            press=1;
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
            key=10;
            press=1;
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
            key=0;
            press=1;
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
            key=11;
            press=1;
            IFS1bits.CNIF = 0; 
            return;
        }
    }       
    
    /* Clear IRQ flag */
    IFS1bits.CNIF = 0;
}


// If 0 -> LED On (Negative logic)

void reset_pulldown(void){
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB2 = 0;
}

//ADC interrupt (needed for displaying current temp in LCD)
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

int start(void) //asterisk start
{
    while(1)
    {
        if (key == 10) //if asterisk was pressed, start
            break;
        else
            Nop();
    }
    return 0;
}
int prompt(void) //user prompt
{
    
    int tot_press=0;
    //int result_temp;
    //char tmp_str[3];
    
    result_temp = floor(adcvalue/3.03); //current temp
    sprintf(tmp_str, "%i", result_temp); //convert int to str
    lcdPrint("Current temp: ");
    delay(2500);
    lcdPrint(tmp_str);
    delay(2500);
    setCursor(0xC0);
    
    lcdPrint("Enter target temp: ");
    delay(2500);
    
    while(tot_press <= 2)
    {
        
        IEC1bits.CNIE = 0; 
        LATBbits.LATB0 = 0;
        LATBbits.LATB1 = 0;
        LATBbits.LATB2 = 0;
        IEC1bits.CNIE = 1;
        
        if (press) {
            tot_press = tot_press+press;
            IEC1bits.CNIE = 0;
        }

        //display current temp in LCD
        if (tot_press == 1)
        {
            sprintf(limit,"%i" ,key);
            lcdPrint(limit);
            delay(2500);
            targ_temp=key*10;
            press = 0;
        }
            
        else if(tot_press == 2)
        {
            sprintf(limit,"%i" ,key);
            lcdPrint(limit);
            delay(2500);
            targ_temp = targ_temp+key;
            break;
        }       
        IEC1BITS.CNIE = 1;
    }    
    return 0;
}

void template() //Target temp vs current temp
{
    clearDisplay();
    
    lcdPrint("Target temp: ");
    delay(2500);
    lcdPrint(limit);
    delay(2500);
    
    result_temp = floor(adcvalue/3.03); //current temp
    sprintf(tmp_str, "%i", result_temp); //convert int to str
    
    lcdPrint("Current temp: ");
    delay(2500);
    lcdPrint(tmp_str);
    delay(2500);
    
}

void PWM_init()
{
    OC1CON = 0;                  //Clear OC1CON1
    OC1R = 0;                     //set value of OC1R
    OC1RS = 40;  
    OC1CON = 0;
    //set value of OC1RS
    //OC1CON2 = 0x020C;             //Set value of OC1CON2
    //OC1CON1 = 0x2008;             //set initial value of OC1CON1 with OC module off
    OC1CONbits.OCM = 0b110;      //set values of OCM bits
    PR2 = 79;                     //set value of PR2
    
}

void ADC_init(){
    AD1PCFG = 0xFDFF;
    AD1CON1 = 0x20E4;
    AD1CON2 = 0x0;
    AD1CON3 = 0x201;
    AD1CHS = 0x9;
    AD1CSSL = 0x0;    
}
void ADC_setup(void){
    __builtin_write_OSCCONL(OSCCON & (~(1<<6)));    // Engage IOlock
    RPOR7bits.RP15R = 0x12;
    RPOR3bits.RP7R = 0x12;     
    __builtin_write_OSCCONL(OSCCON|(1<<6));         // Release IOlock
}