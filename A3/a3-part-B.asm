	.data
KEYBOARD_EVENT_PENDING:
	.word	0x0
KEYBOARD_EVENT:
	.word   0x0
KEYBOARD_COUNTS:
	.space  128
NEWLINE:
	.asciiz "\n"
SPACE:
	.asciiz " "
	
	.eqv 	LETTER_a 97
	.eqv	LETTER_b 98
	.eqv	LETTER_c 99
	.eqv 	LETTER_D 100
	.eqv 	LETTER_space 32
	
	
	.text  
main:
# STUDENTS MAY MODIFY CODE BELOW
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	
	#Please run at max speed
	
	# a known bug: the program will go wrong if we press another key while 
	# interrupts, plus the program ignores the key just press
	# However if we adjust the run speed to max the program will work fine

	la $s0, 0xffff0000	# control register for MMIO Simulator "Receiver"
	lb $s1, 0($s0)
	ori $s1, $s1, 0x02	# Set bit 1 to enable "Receiver" interrupts (i.e., keyboard)
	sb $s1, 0($s0)

	addi $t0, $zero, 0x0000ff11
	
check_for_event:
	la $s0, KEYBOARD_EVENT_PENDING
	lb $s1, 0($s0)
	beq $t7, LETTER_a, adda		
	beq $t7, LETTER_b, addb
	beq $t7, LETTER_c, addc
	beq $t7, LETTER_D, addd
	beq $t7, 32, finish
	beq $s1, $zero, check_for_event
	
adda:		#add 1  to  $s0 if  the key is a  else we ignore them
	addi $s4, $s4, 1
	add $t7, $zero, $zero
	beq $s1, $zero, check_for_event

addb:		#add 1  to  $s1 if  the key is b  else we ignore them
	addi $s5, $s5, 1
	add $t7, $zero, $zero
	beq $s1, $zero, check_for_event

addc:		#add 1  to  $s2 if  the key is c  else we ignore them
	addi $s2, $s2, 1
	add $t7, $zero, $zero
	beq $s1, $zero, check_for_event

addd:		#add 1  to  $s3 if  the key is d  else we ignore them
	addi $s3, $s3, 1
	add $t7, $zero, $zero
	beq $s1, $zero, check_for_event
	
	.kdata		 
			 
	.ktext 0x80000180		 
__kernel_entry:
	mtc0 $12, $t0		# close the interrupt while we precess interrupt (just in case bug )
	mfc0 $k0, $13		# $13 is the "cause" register in Coproc0
	andi $k1, $k0, 0x7c	# bits 2 to 6 are the ExcCode field (0 for interrupts)
	srl  $k1, $k1, 2	# shift ExcCode bits for easier comparison
	beq $zero, $k1, __is_interrupt
	
__is_interrupt:
	andi $k1, $k0, 0x0100	# examine bit 8
	bne $k1, $zero, __is_keyboard_interrupt	 # if bit 8 set, then we have a keyboard interrupt.
	
	beq $zero, $zero, __exit_exception	# otherwise, we return exit kernel

__is_keyboard_interrupt:
	la $k0, 0xffff0004
	lw $t7, 0($k0)		#$t7 figure out what we just press from keyboard

	beq $zero, $zero, __exit_exception	
__exit_exception:			# fix a knownbug if we press a key while mars assamble line 35 
	mfc0 $t2, $14		# the epc ($14) will point at  0x00400028 which will cause infinite loop
	beq $t2, 0x00400028, fixbug	# therefore we add 4 to epc and avoid this issue
	eret
fixbug:
	addi $t2, $t2, 4
	mtc0 $t2, $14
	eret

finish:			# print to ¡°Mars messages¡± pane and end the program
	beq $s4, $zero, finishb
	
	la $a0, concludea
	addi $v0, $zero, 4
	syscall
	
	move $a0, $s4
	addi $v0, $zero, 1
	syscall
	
	la $a0, NEWLINE
	addi $v0, $zero, 4
	syscall
finishb:
	beq $s5, $zero, finishc
	
	la $a0, concludeb
	addi $v0, $zero, 4
	syscall
	
	move $a0, $s5
	addi $v0, $zero, 1
	syscall
	
	la $a0, NEWLINE
	addi $v0, $zero, 4
	syscall
	
finishc:
	beq $s2, $zero, finishd
	
	la $a0, concludec
	addi $v0, $zero, 4
	syscall
	
	move $a0, $s2
	addi $v0, $zero, 1
	syscall
	
	la $a0, NEWLINE
	addi $v0, $zero, 4
	syscall

finishd:
	beq $s3, $zero, end
	
	la $a0, concluded
	addi $v0, $zero, 4
	syscall
	
	move $a0, $s3
	addi $v0, $zero, 1
	syscall
	
	la $a0, NEWLINE
	addi $v0, $zero, 4
	syscall
	
end:		
	add $t7, $zero, $zero
	beq $s1, $zero, check_for_event
	
	
.data 
concludea:
	.asciiz " The total number of  ¡®a¡¯ we pressed:  "
concludeb:
	.asciiz " The total number of  ¡®b¡¯ we pressed:  "
concludec:
	.asciiz " The total number of  ¡®c¡¯ we pressed:  "
concluded:
	.asciiz " The total number of  ¡®d¡¯ we pressed:  "
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# STUDENTS MAY MODIFY CODE ABOVE

	
