.include "p24FJ64GA002.inc"
.global __reset


; PIC24FJ64GA002    
config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF
config __CONFIG2, POSCMOD_NONE & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL ;& PLL96MHZ_OFF & PLLDIV_NODIV    

; PIC24FJ64GB002
;config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF
;config __CONFIG2, POSCMOD_NONE & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV    
.bss
    i: .space 2
    j: .space 2

.text
__reset:
    
    mov #__SP_init, W15
    mov #__SPLIM_init, W0
    mov W0, SPLIM
    mov #0x1, W0
    mov W0, TRISB
    mov #0xFFFF, W0
    mov W0, AD1PCFG
    
/* Main */
CALL	delay15ms
    
MOV	#0x0300, W0		; Function Set Command
CALL	send4ToLCD
CALL	delay15ms
    
CALL	send4ToLCD
CALL	delay15ms
    
CALL	send4ToLCD
CALL	delay15ms
    
MOV	#0x0200, W0		; Sets interface to 4-bit
CALL	send4ToLCD
CALL	delay15ms
    
MOV	#0x0200, W0		; F - 0  || N - 1 (2 lines)
MOV	#0x0800, W1		
CALL	send8ToLCD  
CALL	delay15ms

MOV	#0x0000, W0		; Display Off
MOV	#0x0800, W1
CALL	send8ToLCD 
CALL	delay15ms
 
MOV	#0x0000, W0		; Clear Display
MOV	#0x0100, W1
CALL	send8ToLCD 
CALL	delay15ms
    
mov #0x0000, W0			; Entry Mode 
mov #0x0600, W1
call send8ToLCD 
call delay15ms
    
mov #0x0000, W0			; Display ON
mov #0x0F00, W1
call send8ToLCD 
call delay15ms

    
    
/* Main Loop */    
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
    
    
first_name:  
    mov #0x2500, W0	    ; R
    mov #0x2200, W1         
    call send8ToLCD 
    call delay15ms
    
    mov #0x2700, W0	    ; u
    mov #0x2500, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2700, W0	    ; s
    mov #0x2300, W1
    call send8ToLCD	    
    call delay15ms
    
    mov #0x2700, W0	    ; t
    mov #0x2400, W1
    call send8ToLCD 
    call delay15ms

    mov #0x2700, W0	    ; y
    mov #0x2900, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2A00, W0	    ; space
    mov #0x2000, W1
    call send8ToLCD	    
    call delay15ms

    mov #0x2400, W0	    ; J
    mov #0x2A00, W1         
    call send8ToLCD 
    call delay15ms
    
    mov #0x2600, W0	    ; o
    mov #0x2F00, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2600, W0	    ; h
    mov #0x2800, W1
    call send8ToLCD	    
    call delay15ms
    
    mov #0x2600, W0	    ; n
    mov #0x2E00, W1
    call send8ToLCD 
    call delay15ms

    mov #0x2A00, W0	    ; space
    mov #0x2000, W1
    call send8ToLCD	    
    call delay15ms    
    
    mov #0x2400, W0	    ; L
    mov #0x2C00, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2600, W0	    ; l
    mov #0x2c00, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2600, W0	    ; o
    mov #0x2F00, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2700, W0	    ; y
    mov #0x2900, W1
    call send8ToLCD 
    call delay15ms
    
    mov #0x2600, W0	    ; d
    mov #0x2400, W1
    call send8ToLCD 
    call delay15ms
    return
    
last_name:         
    mov #0x2400, W0	    ; M
    mov #0x2D00, W1
    call send8ToLCD	    
    call delay15ms
    
    mov #0x2600, W0	    ; i
    mov #0x2900, W1
    call send8ToLCD	    
    call delay15ms
    
    mov #0x2600, W0	    ; n
    mov #0x2E00, W1
    call send8ToLCD	    
    call delay15ms

    mov #0x2600, W0	    ; a
    mov #0x2100, W1
    call send8ToLCD	    
    call delay15ms
    return 
        
/* Functions */
send4ToLCD:
    mov W0,PORTB
    bset PORTB, #14	    ; Set Enable 
    bclr PORTB, #14	    ; Clear Enable
    return
    
send8ToLCD:
    mov W0, PORTB
    bset PORTB, #14	    ; Set Enable
    bclr PORTB, #14	    ; Clear Enable
    mov W1, PORTB
    bset PORTB, #14	    ; Set Enable
    bclr PORTB, #14	    ; Clear Enable
    return
    
/* Delays */
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

switch_line:
    MOV #0x0C00, W0		; 0xC0 vs 0x80
    MOV #0x0000, W1
    call send8ToLCD
    call delay15ms
    return

clear_display:
    mov #0x0000, W0		; Clear Display
    mov #0x0100, W1
    call send8ToLCD		
    call delay15ms
    
    mov #0x0000, W0		; Entry Mode Set
    mov #0x0600, W1
    call send8ToLCD		
    call delay15ms
    return
    
end:
.end