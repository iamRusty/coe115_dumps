; ******************************************************************************
; LCD.s - Interface an LCD with PIC24F Microchip
; 
; MIT License
;
; Copyright (c) 2017 Rusty Mina (rusty.mina@eee.upd.edu.ph)
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.
;    
; ******************************************************************************
    
; ***************************** Reference **************************************
;    
; 1. LCD Dot Matrix Manual (Page 28 to 46)
; http://www-inst.eecs.berkeley.edu/~cs150/fa01/labs/project/LCD_User_Manual.pdf
;    
;*******************************************************************************    
    
.include "p24FJ64GA002.inc"
.global __reset

.bss
    i: .space 2    
    
config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF
config __CONFIG2, POSCMOD_NONE & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL   

.text
__reset:
; *************************** Initialize Stack Pointer *************************
MOV	    #__SP_init, W15
MOV	    #__SPLIM_init, W0
MOV	    W0, SPLIM
    
; ***************************** Initialize GPIO ********************************
mov	    #0x0001, W0
mov	    W0, TRISB
mov	    #0xFFFF, W0
mov	    W0, AD1PCFG
    
; ************************** Initialize LCD (4-bit) ****************************
init:    
; No data should be transferred to or from the display for 15ms    
    CALL    delay15ms
; Function Set Command: (8-Bit interface)   
    MOV	    #0x0003, W0		
    CALL    send4ToLCD
    CALL    delay4_1ms
    
    CALL    send4ToLCD
    CALL    delay100us

; After this command is written, BF can be checked    
    CALL    send4ToLCD
    CALL    delay15ms

; Function Set: Sets interface to 4-bit    
    MOV	    #0x0002, W0		
    CALL    send8ToLCD
    CALL    delay15ms

; Function Set (Interface = 4-bit, 
;   Set N for number of lines	(1)
;   Set F for character font    (0)
    MOV	    #0x0028, W0		
    CALL    send8ToLCD		
    call    delay15ms		

; Display OFF    
    MOV	    #0x0008, W0		
    CALL    send8ToLCD
    CALL    delay15ms

; Clear Display    
    MOV	    #0x0001, W0		
    CALL    send8ToLCD
    CALL    delay15ms

; Entry Mode Set   
    MOV	    #0x0006, W0		
    CALL    send8ToLCD
    call    delay15ms

; Display ON
;   Set C for Cursor	(1)
;   Set B for Blink	(1)
    MOV	    #0x000F, W0		
    CALL    send8ToLCD
    CALL    delay15ms    
; ==============================================================================
    
; ************************** Main Program Loop *********************************
fl:
    call clear_display
    call first_name
    call switch_line
    call last_name
    poll1:
	mov PORTB, W7  
	btsc W7, #0	    ; skip next if PORTB<0> is 0
	goto poll1
	goto lf
	
lf:
    call clear_display
    call last_name
    call switch_line
    call first_name
    poll2:
	mov PORTB, W7
	btss W7, #0	    ; skip next if PORTB<0> is 1
	goto poll2
	goto fl       
; ==============================================================================

; ********************************** Functions *********************************	
last_name:
    mov #0x4D, W0     ; M
    call send8ToLCD_data
    call delay15ms
    
    mov #0x69, W0     ; i
    call send8ToLCD_data
    call delay15ms
    
    mov #0x6E, W0     ; n
    call send8ToLCD_data
    call delay15ms
    
    mov #0x61, W0     ; a
    call send8ToLCD_data
    call delay15ms 
    return    
    
first_name:
    mov #0x52, W0     ; R
    call send8ToLCD_data
    call delay15ms
    
    mov #0x75, W0     ; u
    call send8ToLCD_data
    call delay15ms
    
    mov #0x73, W0     ; s
    call send8ToLCD_data
    call delay15ms
    
    mov #0x74, W0     ; t
    call send8ToLCD_data
    call delay15ms
    
    mov #0x79, W0     ; y
    call send8ToLCD_data
    call delay15ms
    return 
    
; Used only in initialization    
send4ToLCD:
    SL	    W0, #8, W0
    MOV	    W0,	PORTB
    nop
    BSET    PORTB, #14	    ; Set Enable
    nop
    BCLR    PORTB, #13	    ; Clear Enable
    return

; Send 8-bit Instruction
send8ToLCD:
    MOV	    #0x00F0, W7
    AND	    W7, W0, W7
    SL	    W7, #4, W7
    MOV	    #0x000F, W8
    AND	    W8, W0, W8
    SL	    W8, #8, W8
    MOV	    W7, PORTB
    nop
    BSET    PORTB, #14	    
    nop
    BCLR    PORTB, #14
    nop
    MOV	    W8, PORTB
    nop
    BSET    PORTB, #14
    nop
    BCLR    PORTB, #14
    nop
    return

; Send 8-bit Data    
send8ToLCD_data:
    MOV	    #0x00F0, W7
    AND	    W7, W0, W7
    SL	    W7, #4, W7
    MOV	    #0x000F, W8
    AND	    W8, W0, W8
    SL	    W8, #8, W8
    BSET    W7, #13
    MOV	    W7, PORTB
    nop
    BSET    PORTB, #14
    nop
    BCLR    PORTB, #14
    nop
    BSET    W8, #13
    MOV	    W8, PORTB
    nop
    BSET    PORTB, #14
    nop
    BCLR    PORTB, #14
    nop
    return    

; Function: Set Cursor to 2nd line    
switch_line:
    MOV #0x00C0, W0		; 0xC0 - 2nd line
    call send8ToLCD		; 0x80 - 1st line
    call delay15ms
    return

; Function: Clear Display AND Entry Mode Set    
clear_display:
    mov #0x0001, W0		
    call send8ToLCD		
    call delay15ms
    
    mov #0x0006, W0		
    call send8ToLCD		
    call delay15ms
    return    
    
; ==============================================================================
    
; ********************************** Delays ************************************
delay15ms:
    mov #0x2710, W0
    mov W0, i
    
loop15ms:
    dec i
    bra nz, loop15ms
    return

delay4_1ms:
    mov #0xAB0, W0
    mov W0, i
    
loop4_1ms:
    dec i
    bra nz, loop4_1ms
    return

delay100us:
    mov #0x41, W0
    mov W0, i
    
loop100us:
    dec i
    bra nz, loop100us
    return
; ==============================================================================
    
end:
.end

