.include "p24FJ64GB002.inc"
.global __reset
    
.bss
    i: .space 2
    j: .space 2

.text
__reset:
    
mov #__SP_init, W15
mov #__SPLIM_init, W0
mov W0, SPLIM
    
    
/* Main */
MOV	#0x03, W0
CALL	send4ToLCD
CALL	delay4_1ms
    
CALL	send4ToLCD
CALL	delay100us
    
CALL	send4ToLCD
MOV	#0x02, W0
CALL	send4ToLCD

MOV	#0x28, W0
CALL	send8ToLCD
    
MOV	#0x04, W0
CALL	send8ToLCD
    
MOV	#0x01, W0
CALL	send8ToLCD

MOV	#0x06, W0
CALL	send8ToLCD
    
MOV	#0x0F, W0
CALL	send8ToLCD

    
    
main:
    
    
/* Functions */
send4ToLCD:
    /* Extract RS Bit and store in PORTB<13> */
    btsc W0, #4		; Skip next instruction if W0<4> is 0
    goto RS_1_LCD_4 
    bclr PORTB, #13
    goto done_with_RS_LCD_4
    RS_1_LCD_4:
    bset PORTB, #13
    done_with_RS_LCD_4:
    bset PORTB, #14	; Set E bit
    /* Sketchy Code */
    MOV PORTB, W3	
    MOV #0xF0FF, W4
    AND W4, W3, W3	; Preserve value of PORTB except for PORTB[11:8]
    MOV #0x000F, W5
    AND W5, W0, W5	; Extract W0[3:0] bits
    SL	W5, #8, W5	; Shift W0[3:0] -> W0[11:8]
    IOR W5, W3, W3
    MOV W3, PORTB
    bclr PORT, #14
    return
    
send8ToLCD:
    /* Extract RS Bit and store in PORTB<13> */
    btsc W0, #8		; Skip next instruction if W0<8> is 0
    goto RS_1_LCD_8 
    bclr PORTB, #13
    goto done_with_RS_LCD_8
    RS_1_LCD_8:
    bset PORTB, #13
    done_with_RS_LCD_8:
    bset PORTB, #14	; Set E bit
    /* Another Sketchy Code */
    MOV PORTB, W3
    MOV #0xF0FF, W4
    AND W4, W3, W3	; Preserve value of PORTB except for PORTB[11:8]
    MOV #0x00F0, W5	
    AND W5, W0, W5	; Extract W0[7:4]
    SL	W5, #4, W5	; Shift W0[7:4] to W[11:8]
    IOR W3, W5, W3	
    MOV W3, PORTB	; Send DB[7:4] Bits
    bclr PORTB, #14	; Reset E Bit
    
    bset PORTB, #14
    MOV PORTB, W3
    MOV #0xF0FF, W4
    AND W4, W3, W3	; Preserve value of PORTB except for PORTB[11:8]
    MOV #0x000F, W5
    AND W5, W0, W5	; Extract W[3:0]
    SL	W5, #8, W5	; Shift W0[3:0] TO W0[11:8]
    IOR W3, W5, W3
    MOV W3, PORTB	; Send DB[3:0]
    bclr PORTB, #14
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
        
.end
    
    




