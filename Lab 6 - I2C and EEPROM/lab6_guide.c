/*
 * File:   lab6.c
 * Author: sago
 *
 * Created on March 6, 2017, 4:42 PM
 */

#include "xc.h"

/* if PIC24FJ64GA002 */
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config I2C1SEL=PRI  


/* else if PIC24FJ64GB002
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=SOSC
#pragma config I2C1SEL=PRI 
 */

/* else if PIC24FJ64GA202
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMD=NONE, OSCIOFCN=ON, FCKSM=CSDCMD, FNOSC=FRC 
#pragma config PLLDIV=DISABLED
#pragma config SOSCSEL=ON
#pragma config I2C1SEL=DISABLE
*/


#define BUFMAX 32

void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void);

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_tx(int data);
int i2c_rx(void);
void wait_idle(void);
void delay (long millisecs);
unsigned long hash(char *str);
void send_ack(void);

// data vars
char indata[] = "CoE_115";
char outdata[BUFMAX]; 

// flags
int idle = 1;
int acked = 0;
int rx_complete = 0;
int tx_complete = 0;

int main(void) {
    
    i2c_init();

    /* Write to EEPROM
     * Store "CoE_115" in address 0x1000 of the EEPROM 
     */
    // send I2C start sequence
    // send EEPROM CTRL byte with Write enabled
    // send EEPROM high address
    // send EEPROM low address 
    // send actual data; 
    // send I2C stop sequence

    delay(); //5ms delay

    /* Read from EEPROM
     * Get data written previously in the EEPROM
     */
    // send I2C start sequence
    // send EEPROM CTRL byte with Write enabled
    // send EEPROM high address
    // send EEPROM low address
    // send I2C start sequence
    // send EEPROM CTRL byte with Read enabled
    // receive data
    // send ACK, but not on last byte
        // otherwise I2C wouldn't be functional
        // until you perform a hard reset/ power on-off
    // send I2C stop sequence

           
    while(1){   
        // Display data
    }
    
    return 0;
}

void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void){
    
   
    if (/*ACK received */){  
        acked = 1;
    }
    
    if (/* RX completed */){      
        rx_complete = 1;
    }
    
    if (/*TX completed*/){     
        tx_complete = 1;
    }
    if (/* bus is idle or no write collision */){    
        idle = 1;
    }
    
    // Clear I2C interrupt flag
}

void reset_flags(){
    idle = 1;
    tx_complete = 0;
    rx_complete = 0;
    acked = 0;
}

void wait_idle(){
    while (!idle){
        // do nothing, just wait
    }
    reset_flags();
}

void delay (long millisecs) {
    // implement delay
}

void i2c_start(){
    //wait until free
    //initiate start condition on SDA and SCL pins
}

void i2c_stop(){
    //wait until free
    //initiate stop condition on SDA and SCL pins
}

void i2c_init(void){
       
    // Configure GPIOs

    // ##### Interrupt-driven I2C configuration ########
    // enable I2C interrupts
    // clear interrupt flag for I2C
    
    // ##### I2C configuration #########################
    // refer to notes in Lab06 specs
    
    // enable I2C module
}

void i2c_tx(int data){
    
    // wait until free
    // set flag to declare you're not free/idle
    // load data for transmission over the I2C bus
    // wait until TX is complete
    // wait until ACK is received
    // reset flags
}

int i2c_rx(){
    
    int temp = 0;
 
    // The lower 5 bits of I2CxCON must be '0' before attempting to set the RCEN bit. 
    // This ensures the master logic is inactive.
    
    // enable RX mode     
    // wait until free
    // set flag to declare you're not free/idle
    // wait until RX is complete
    // get our data, store in temp
    // reset flags
    
    return temp;
}

unsigned long hash(char *str){
        
    unsigned long hash = 5381;
    int c;

    // djb2 hash algorithm
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; 

    return hash;
}

void send_ack(void){
    // wait until free
    // set flag to declare you're not free/idle
    // send acknownledgement
    // reset idle flag
}
