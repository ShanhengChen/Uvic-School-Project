# This code assumes the use of the "Bitmap Display" tool.
#
# Tool settings must be:
#   Unit Width in Pixels: 32
#   Unit Height in Pixels: 32
#   Display Width in Pixels: 512
#   Display Height in Pixels: 512
#   Based Address for display: 0x10010000 (static data)
#
# In effect, this produces a bitmap display of 16x16 pixels.


	.include "bitmap-routines.asm"

	.data
TELL_TALE:
	.word 0x12345678 0x9abcdef0	# Helps us visually detect where our part starts in .data section
KEYBOARD_EVENT_PENDING:
	.word	0x0
KEYBOARD_EVENT:
	.word   0x0
BOX_ROW:
	.word	0x0
BOX_COLUMN:
	.word	0x0

	.eqv LETTER_a 97
	.eqv LETTER_d 100
	.eqv LETTER_w 119
	.eqv LETTER_x 120 	 # X OR S?
	.eqv BOX_COLOUR 0x0099ff33
	
	.globl main
	
	.text	
main:
# STUDENTS MAY MODIFY CODE BELOW
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	# Please run at max speed
	#basic idea of part D in a3
	# check pressed keys epual any direction letter (97, 100,115,119) meanwhile interrupt occur
	# if so, store adjustion in $t2 then back to check_for_event loop
	# firstly, clear the original fourbyfour pixie by setting a2 into box_color_black and jal  draw_bitmap_box
	# secondly, after the box disapears, update the new row/column by move $a0/a1 , $t2
	# lastly, draw the new coordinate of box, continue the loop and wait for new  direction letter 
	
	addi $a0, $zero, 0
	addi $a1, $zero, 0
	addi $a2, $zero, 0x0099ff33
	addi $t1, $t1, 0x00000000
	addi $t3, $t3, 0x0099ff33
	addi $t4, $t4, 0x00400110
	la $s0, 0xffff0000	# control register for MMIO Simulator "Receiver"
	lb $s1, 0($s0)
	ori $s1, $s1, 0x02	# Set bit 1 to enable "Receiver" interrupts (i.e., keyboard)
	sb $s1, 0($s0)

	jal draw_bitmap_box	
check_for_event:
	nop
	beq $t7, 97, add_and_remove_left
	beq $t7, 100, add_and_remove_right
	beq $t7, 119, add_and_remove_up
	beq $t7, 115, add_and_remove_down
	beq $zero, $zero, check_for_event
	

add_and_remove_left:
	addi $t2,$a1, -1
	b add_and_remove1
	
add_and_remove_right:
	addi $t2,$a1, 1
	b add_and_remove1
add_and_remove1:
	move $a2, $t1
	move $s1, $a0
	jal draw_bitmap_box
	move $a0, $s1
	move $a1, $t2
	move $a2, $t3
	jal draw_bitmap_box
	add $t7, $zero, $zero
	beq $zero, $zero, check_for_event
	
add_and_remove_up:
	addi $t2,$a0, -1
	b add_and_remove2
add_and_remove_down:
	addi $t2,$a0, 1
	b add_and_remove2
add_and_remove2:
	move $a2, $t1
	move $s2, $a1
	jal draw_bitmap_box
	move $a1, $s2
	move $a0, $t2	
	move $a2, $t3
	jal draw_bitmap_box
	add $t7, $zero, $zero
	beq $zero, $zero, check_for_event
	
	
	# Should never, *ever* arrive at this point
	# in the code.	

	addi $v0, $zero, 10

.data
    .eqv BOX_COLOUR_BLACK 0x00000000
.text

	addi $v0, $zero, BOX_COLOUR_BLACK
	syscall



# Draws a 4x4 pixel box in the "Bitmap Display" tool
# $a0: row of box's upper-left corner
# $a1: column of box's upper-left corner
# $a2: colour of box

draw_bitmap_box:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	addi $t6, $t6, 3	 
	addi $t5, $t5, 4	 

loop:
	beqz $t5 row_addone
	jal set_pixel
	addi $t5, $t5, -1
	addi $a1, $a1, 1
	b loop
	
row_addone:
	beqz $t6 finish
	
	addi $t5, $t5, 4
	addi $a1, $a1, -4
	addi $a0, $a0,  1
	addi $t6, $t6, -1
	b loop
finish:	
	addi $a0, $a0,  -3
	addi $a1, $a1,  -4
	lw $ra, 0($sp)
	addi $sp, $sp, 4 
	jr $ra


	.kdata

	.ktext 0x80000180
#
# You can copy-and-paste some of your code from part (a)
# to provide elements of the interrupt handler.
#
__kernel_entry:
	mtc0 $12, $t0		# close the interrupt while we precess interrupt (just in case bug )
	mfc0 $k0, $13		# $13 is the "cause" register in Coproc0
	andi $k1, $k0, 0x7c	# bits 2 to 6 are the ExcCode field (0 for interrupts)
	srl  $k1, $k1, 2	# shift ExcCode bits for easier comparison
	beq $zero, $k1, __is_interrupt
	
__is_interrupt:
	andi $k1, $k0, 0x0100		# examine bit 8
	bne $k1, $zero, __is_keyboard_interrupt	 # if bit 8 set, then we have a keyboard interrupt.
	
	beq $zero, $zero, __exit_exception	# otherwise, we return exit kernel

__is_keyboard_interrupt:
	la $k0, 0xffff0004
	lw $t7, 0($k0)	

__exit_exception:
	mtc0 $t4, $14	# just in case bug loop here 
	eret
.data

# Any additional .text area "variables" that you need can
# be added in this spot. The assembler will ensure that whatever
# directives appear here will be placed in memory following the
# data items at the top of this file.

	
# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# STUDENTS MAY MODIFY CODE ABOVE


.eqv BOX_COLOUR_WHITE 0x00FFFFFF
	
