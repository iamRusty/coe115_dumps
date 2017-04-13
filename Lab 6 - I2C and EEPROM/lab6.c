#include "xc.h"
#include "include/lcd_4bit.h"


/* PIC24GB002 */
#pragma config FWDTEN=OFF, JTAGEN=OFF
#pragma config POSCMOD=NONE, OSCIOFNC=ON, FCKSM=CSDCMD, FNOSC=FRCPLL 
#pragma config PLL96MHZ=OFF, PLLDIV=NODIV
#pragma config SOSCSEL=SOSC
#pragma config I2C1SEL=PRI 

#define BUFMAX 32

void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void);

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_tx(int data);
int i2c_rx(void);
void wait_idle(void);
void send_ack();
//void delay (long millisecs);          // Already defined in lcd_4bit library
unsigned long hash(char *str);

// data vars
char indata[] = "CoE_115";
char outdata[BUFMAX]; 

// flags
int idle = 1;
int acked = 0;
int rx_complete = 0;
int tx_complete = 0;

int main(void) {
    AD1PCFG = 0xFFFF;
    TRISB = 0x9FF0;
    
    i2c_init();
    lcdInit();
    /* Write to EEPROM
     * Store "CoE_115" in address 0x1000 of the EEPROM 
     */
    // send I2C start sequence
    i2c_start();

    // send EEPROM CTRL byte with Write enabled
    i2c_tx(0b10100000);

    // send EEPROM high address
    i2c_tx(0x10);

    // send EEPROM low address
    i2c_tx(0x00); 

    // send actual data;
    int _count_char_ = 0;
    while (_count_char_ < 7){
        i2c_tx(indata[_count_char_]); 
        _count_char_++;
    }
    i2c_tx(0);

    // send I2C stop sequence
    i2c_stop();

    delay(5); //5ms delay

    /* Read from EEPROM
     * Get data written previously in the EEPROM
     */

    // send I2C start sequence
    i2c_start();
    // send EEPROM CTRL byte with Write enabled
    i2c_tx(0b10100000);
    // send EEPROM high address
    i2c_tx(0x10);
    // send EEPROM low address
    i2c_tx(0x00);
    // send I2C start sequence
    i2c_start();
    // send EEPROM CTRL byte with Read enabled
    i2c_tx(0b10100001);

    // receive data
    char lcdString[8];
    _count_char_ = 0;
    while (_count_char_ < 8){
        lcdString[_count_char_] = i2c_rx();
        _count_char_++;

        // send ACK, but not on last byte
        if (_count_char_ < 7)
        send_ack();
    }
    lcdString[_count_char_ - 1] = 0;
    
    // send I2C stop sequence
    i2c_stop();

    // print on next line   
    setCursor(0xC0);
    lcdPrint(lcdString);

    while(1){   
        // Display data
    }

    return 0;
}

void __attribute__((interrupt, auto_psv)) _MI2C1Interrupt(void){
    
    // ACK received
    if (!I2C1STATbits.ACKSTAT){  
        acked = 1;
    }
    
    // RX completed
    if (I2C1STATbits.RBF){      
        rx_complete = 1;
    }
    
    // TX completed
    if (!I2C1STATbits.TBF){     
        tx_complete = 1;
    }

    // bus is idle or no write collision
    if (!I2C1STATbits.IWCOL || !I2C1STATbits.BCL){    
        idle = 1;
    }
    
    // Clear I2C interrupt flag
    IFS1bits.MI2C1IF=0;
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


// delay function already defined in lcd_4bit library
/*
void delay (long millisecs) {
    int count, count2;

    for (count2 = 0; count2 < millisecs; count2++){
        for (count = 0; count < 498; count++){
        Nop();
        Nop();
        }
    }
    return;
}*/

void i2c_start(){
    //wait until free
    wait_idle();
    //initiate start condition on SDA and SCL pins
    I2C1CONbits.SEN = 1;
}

void i2c_stop(){
    //wait until free
    wait_idle();
    //initiate stop condition on SDA and SCL pins
    I2C1CONbits.PEN = 1;
}

void i2c_init(void){
       
    // Configure GPIOs

    // ##### Interrupt-driven I2C configuration ########
    // enable I2C interrupts
    IEC1bits.MI2C1IE = 1;

    // clear interrupt flag for I2C
    IFS1bits.MI2C1IF = 0;
    // ##### I2C configuration #########################
    // refer to notes in Lab06 specs
    I2C1CONbits.I2CSIDL = 1;        // Stop in Idle mode
    I2C1CONbits.ACKDT = 0;          // Sends ACK during Acknowledge
    I2C1BRG = 4;                    // FSCL = 400 kHz
    
    // enable I2C module
    I2C1CONbits.I2CEN = 1;
}

void i2c_tx(int data){
    //TRISB = 0;
    // wait until free
    wait_idle();

    // set flag to declare you're not free/idle
    idle = 0;
    // load data for transmission over the I2C bus
    I2C1TRN = data;

    // wait until TX is complete
    while(!tx_complete){
        // do nothing, just wait
    }
    // wait until ACK is received
    while(!acked){
        // do thing, just wait
    }
    // reset flags
    reset_flags();
}

int i2c_rx(){
    int temp = 0;
    
    // The lower 5 bits of I2CxCON must be '0' before attempting to set the RCEN bit. 
    // This ensures the master logic is inactive.
    I2C1CONbits.ACKEN = 0;      // bit 4
    I2C1CONbits.RCEN = 0;       // bit 3
    I2C1CONbits.PEN = 0;        // bit 2
    I2C1CONbits.RSEN = 0;       // bit 1
    I2C1CONbits.SEN = 0;        // bit 0

    // enable RX mode     
    I2C1CONbits.RCEN = 1;       
    
    // wait until free
    wait_idle();

    // set flag to declare you're not free/idle
    idle=0;
     
    // wait until RX is complete
    while(!rx_complete){
        // do nothing
    }
    // get our data, store in temp
    temp=I2C1RCV;

    // reset flags
    reset_flags();
    
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

void send_ack(){
    // Wait until free
    wait_idle();
    
    // NOT FREE
    idle = 0;
    
    // Acknowledge
    I2C1CONbits.ACKEN = 1;

    // reset idle flag
    idle = 1;
}