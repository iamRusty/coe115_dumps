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
    
    clr i
    clr j
   
    
    mov #0x0001, W0
    mov W0, TRISB
    
call delay_15m

mov #0x3, W0
call send4ToLCD
call delay4_1ms
    
call send4ToLCD
call delay100us
    
call send4ToLCD
mov #0x2, W0
call send4ToLCD
    
mov #0x28, W0
call send8ToLCD
    
mov #0x08, W0
call send8ToLCD
    
mov #0x01, W0
call send8ToLCD
    
mov #0x06, W0
call send8ToLCD
    
mov #0x0F, W0
call send8ToLCD
   
call write_first_name
    mov #0xC0, W0
    call send8ToLCD
call write_last_name

loop_main:
    mov PORTB, W0	;get input from pb
    mov #0x0001, W1
    and W0, W1, W0
    
    cp W0, #0x1
    bra LT, switch_lines 
    bra loop_main
    
bra end
    
switch_lines:
    mov i, W0
    cp W0, #1
    bra LT, write_fname_to_line2    ;if fname in line 1 then write fname to line 2
    bra write_fname_to_line1	    ;else write fname to line 1
    
    return
    
    write_fname_to_line1:
	mov #0xC0, W0	; go to line 2
	call send8ToLCD
	call write_last_name
	mov #0x80, W0 ; go to line 1
	call send8ToLCD
	call write_first_name
	btg i, #0
	return
    
    write_fname_to_line2:
	mov #0xC0, W0	; go to line 2
	call send8ToLCD
	call write_first_name
	mov #0x80, W0 ; go to line 1
	call send8ToLCD
	call write_last_name
	btg i, #0
	return



	
    
write_first_name:
    bset PORTB, #13 ;write mode on
    
    mov #0x55, W0	;0101 0101 U
    call send8ToLCD

    mov #0x79, W0	;0111 1010 z
    call send8ToLCD

    mov #0x69, W0	;0110 1001 i
    call send8ToLCD

    mov #0x65, W0	;0110 0101 e
    call send8ToLCD

    mov #0x6C, W0	;0110 1100 l  
    call send8ToLCD

    mov #0xA0, W0	;1010 0000 " "
    call send8ToLCD

    mov #0x52, W0	;0101 0010 R
    call send8ToLCD

    mov #0x65, W0	;0110 0101 e
    call send8ToLCD

    mov #0x69, W0	;0110 1001 i
    call send8ToLCD

    mov #0x6E, W0	;0110 1110 n
    call send8ToLCD

    bclr PORTB, #13	;write mode off

    return
    
write_last_name:
    bset PORTB, #13	;write mode on
    
    mov #0x41, W0	;0100 0001 A
    call send8ToLCD

    mov #0xE7, W0	;1110 0111 g
    call send8ToLCD

    mov #0x75, W0	;0111 0101 u
    call send8ToLCD

    mov #0x62, W0	;0110 0010 b
    call send8ToLCD
    
    bclr PORTB, #13	;write mode off

    return
    
send4ToLCD:
    bset PORTB, #14
    
    sl W0, #8, W0
    mov #0xF0FF, W1
    ior W0, W1, W0
    mov PORTB, W1
    and W0, W1, W0
    mov W0, PORTB
    
    bclr PORTB, #14
    return
    
send8ToLCD:
    bset PORTB, #14
    
    lsr W0, #4, W1
    sl W1, #8, W1
    mov #0xF0FF, W2
    ior W1, W2, W1
    mov PORTB, W2
    and W1, W2, W1
    mov W1, PORTB
    
    sl W0, #8, W0
    mov #0x0F00, W1
    and W0, W1, W0
    mov #0xF0FF, W1
    ior W0, W1, W0
    mov PORTB, W1
    and W0, W1, W0
    mov W0, PORTB
    
    bclr PORTB, #14
    return
    
    
    
    
delay_15m:
    mov #0x0001, W0
    mov W0, i
    mov #0x2800, W0
    mov W0, j
    bra loop
    
delay4_1ms:
    mov #0x0001, W0
    mov W0, i
    mov #0x0B00, W0
    mov W0, j
    bra loop
    
delay_40u:
    mov #0x0001, W0
    mov W0, i
    mov #0x001A, W0
    mov W0, j
    bra loop
    
delay100us:
    mov #0x0001, W0
    mov W0, i
    mov #0x0040, W0
    mov W0, j
    bra loop
    
loop:
    dec j
    bra nz, loop
    dec i
    bra nz, loop
    return
end:
.end