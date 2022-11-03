.text


main:	



# STUDENTS MAY MODIFY CODE BELOW
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

	## Test code that calls procedure for part A
	#jal save_our_souls

	## morse_flash test for part B
	#addi $a0, $zero, 0x42   # dot dot dash dot
	# jal morse_flash
	
	## morse_flash test for part B
	#addi $a0, $zero, 0x37   # dash dash dash
	#jal morse_flash
		
	## morse_flash test for part B
	#addi $a0, $zero, 0x32  	# dot dash dot
	# jal morse_flash
			
	## morse_flash test for part B
	#addi $a0, $zero, 0x11   # dash
	#jal morse_flash	
	
	# flash_message test for part C
	#la $a0, test_buffer
	#jal flash_message
	
	# letter_to_code test for part D
	#the letter 'P' is properly encoded as 0x46.
	#addi $a0, $zero, 'P'
	# jal letter_to_code
	
	# letter_to_code test for part D
	# the letter 'A' is properly encoded as 0x21
	#addi $a0, $zero, 'A'
	#jal letter_to_code
	
	# letter_to_code test for part D
	# the space' is properly encoded as 0xff
	 #addi $a0, $zero, ' '
	# jal letter_to_code
	
	# encode_message test for part E
	# The outcome of the procedure is here
	# immediately used by flash_message
	# la $a0, message01
	 #la $a1, buffer01
	# jal encode_message
	# la $a0, buffer01
	# jal flash_message
	
	
	# Proper exit from the program.
	addi $v0, $zero, 10
	syscall

	
	
############################################# A
# PROCEDURE
display_dot:
	add $sp, $sp, -4
	sw $ra, 0($sp)
	
	jal seven_segment_on
	jal delay_short
	jal seven_segment_off
	jal delay_long
	
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra

display_dash:
	add $sp, $sp, -4
	sw $ra, 0($sp)
	
	jal seven_segment_on
	jal delay_long
	jal seven_segment_off
	jal delay_long
	
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra

save_our_souls:
	add $sp, $sp, -4
	sw $ra, 0($sp)
		
	jal display_dot
	jal display_dot
	jal display_dot
	
	jal display_dash
	jal display_dash
	jal display_dash

	jal display_dot
	jal display_dot
	jal display_dot	
	
	lw $ra, 0($sp)
	add $sp, $sp, 4
	jr $ra

###################################### B
# PROCEDURE
morse_flash:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	li $t0,  0xf	# ob11110000
	li $t4, 0xf0	#ob00001111
	li $t5, 0x1	#ob00000001	
	li $t7,  0xff
	add $s1, $zero, $a0
	and $s1, $s1, $t4	# turn high nybble into low nybble and save in $t1
	srl $s1, $s1, 4
	add $s2, $zero, $a0
	and $s2, $s2, $t0	#mark high nybble and  save low nybble into $t2
	
	beq $a0, $t7, special_case
loop:	
	beqz $s1, done
	and $t6, $s2, $t5	#$t6 determine whatever the rightmost bit of $t2 has 1
	beq $t6, $zero, displaydots
	jal display_dash
	srl $s2, $s2, 1
	addi $s1, $s1, -1
	j loop
displaydots:
 	jal display_dot 
 	srl $s2, $s2, 1
	addi $s1, $s1, -1
	j loop
done:	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra

special_case:
	jal delay_long
	jal delay_long
	jal delay_long
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra

########################################### C
# PROCEDURE
flash_message:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	li $t9, 0		# $t6 == end of the sequence
	move $s0, $a0		#$s0 store the address of test_buffer
	lbu  $a0, 0($s0)
loop1:	
	beq $a0, $t9, finish	#while $a0 == null then end procedure 		
	jal morse_flash	
	addi $s0, $s0, 1
	lbu  $a0, 0($s0)
	j loop1

finish:		
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra


####################################### D
# PROCEDURE
letter_to_code:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	li $s3, 0x00000000		#initialize the low nybbile answer
	li $s5, 0x1		# use for and op	
	li $s6, 0x0		# final answer
	add $s4, $zero, $zero	#initialize the counter for sequence and finally convert into high nybbile for $V0
	la $s1, codes
	li $s7, 209
	li $t1, 32
loop2:	
	lbu $s2, 0($s1)	
	beq $s7, $zero, out_of_index	# loop until we find the Capiltalize character like 'A'
	beq  $s2, $t1, blank_space
	beq $a0, $s2, start_encoding	# define a counter for the loop if the counter is greater 26*8 = 208 then return 0xff
	addi $s1,$s1,1
	addi $s7,$s7,-1
	j loop2
	
blank_space:
	addi $v0, $zero, 0xff	
	lw $ra, 0($sp)		
	addi $sp, $sp, 4
	jr $ra
	
out_of_index:
	addi $v0, $zero, 0xff	# ADD FINAL ANSWER FOMR $6 INTO $V0
	lw $ra, 0($sp)		
	addi $sp, $sp, 4
	jr $ra
start_encoding:
	addi $s1,$s1,1
	lbu $s2, 0($s1) 	#  ASCII Code for '-'  = 45  ASCII Code for '.'  = 46
	beq $s2, $zero, reverse
	beq $s2, 45, equal_dash
	sll $s5,$s5,1
	addi $s4, $s4, 1
	j start_encoding

equal_dash:
	or $s3, $s3, $s5
	sll $s5,$s5,1
	addi $s4, $s4, 1
	j start_encoding

reverse:	
	li $t9, 0x1		#use # t9 for or op for $s6 and shit_loop just point 0x1 
	add $s7, $s4, $zero	# $t9 track the 1 bit reversely and $t6 track 1 bit in sequence order
	add $t6, $zero, $t9		#counter for shift_loop
shift_loop:	
	addi $s7, $s7, -1		#afterthe loop we get a pointer $t6 that points at bit position where $s3 point
	beq $s7, $zero, next		
	sll $t6, $t6, 1		#$s6 record the 1 bit that respond from $s9 
	j shift_loop

next:	
	add $s7, $s4, $zero	# now reverse the stuff
or_loop:
	beq $s7, $zero, finish2	
	and $t5, $t6, $s3
	beq $t5, $zero, dont_have_1
	or $s6, $s6, $t9		# position have one
	srl $t6, $t6, 1
	sll $t9, $t9, 1
	addi $s7, $s7, -1
	j or_loop
dont_have_1:
	srl $t6, $t6, 1
	sll $t9, $t9, 1
	addi $s7, $s7, -1
	j or_loop
	
finish2:
	sll  $s4, $s4, 4
	or $s6, $s4, $s6
	add $v0, $zero, $s6	# ADD FINAL ANSWER FOMR $6 INTO $V0
	lw $ra, 0($sp)		
	addi $sp, $sp, 4
	jr $ra


################################################ E
	# la $a0, message01
	# la $a1, buffer01
	#jal encode_message
	# la $a0, buffer01
	# jal flash_message
# PROCEDURE
encode_message:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	add $t7, $zero, $a0	# store the address of messagexx into $s1 
	lbu $a0, 0($t7)

		

loop3:
	jal letter_to_code
	sb $v0, 0($a1)
	addiu $t7, $t7, 1
	addiu $a1, $a1, 1
	lbu $a0, 0($t7)
	beq $a0, $zero, message_finish
	j loop3

 message_finish:	
	lw $ra, 0($sp)		
	addi $sp, $sp, 4
	jr $ra


# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# STUDENTS MAY MODIFY CODE ABOVE

#############################################
# DO NOT MODIFY ANY OF THE CODE / LINES BELOW

###########
# PROCEDURE
seven_segment_on:
	la $t1, 0xffff0010     # location of bits for right digit
	addi $t2, $zero, 0xff  # All bits in byte are set, turning on all segments
	sb $t2, 0($t1)         # "Make it so!"
	jr $31


###########
# PROCEDURE
seven_segment_off:
	la $t1, 0xffff0010	# location of bits for right digit
	sb $zero, 0($t1)	# All bits in byte are unset, turning off all segments
	jr $31			# "Make it so!"
	

###########
# PROCEDURE
delay_long:
	add $sp, $sp, -4	# Reserve 
	sw $a0, 0($sp)
	addi $a0, $zero, 600
	addi $v0, $zero, 32
	syscall
	lw $a0, 0($sp)
	add $sp, $sp, 4
	jr $31

	
###########
# PROCEDURE			
delay_short:
	add $sp, $sp, -4
	sw $a0, 0($sp)
	addi $a0, $zero, 200
	addi $v0, $zero, 32
	syscall
	lw $a0, 0($sp)
	add $sp, $sp, 4
	jr $31

#############
# DATA MEMORY
.data
codes:
	.byte 'A', '.', '-', 0, 0, 0, 0, 0
	.byte 'B', '-', '.', '.', '.', 0, 0, 0
	.byte 'C', '-', '.', '-', '.', 0, 0, 0
	.byte 'D', '-', '.', '.', 0, 0, 0, 0
	.byte 'E', '.', 0, 0, 0, 0, 0, 0
	.byte 'F', '.', '.', '-', '.', 0, 0, 0
	.byte 'G', '-', '-', '.', 0, 0, 0, 0
	.byte 'H', '.', '.', '.', '.', 0, 0, 0
	.byte 'I', '.', '.', 0, 0, 0, 0, 0
	.byte 'J', '.', '-', '-', '-', 0, 0, 0
	.byte 'K', '-', '.', '-', 0, 0, 0, 0
	.byte 'L', '.', '-', '.', '.', 0, 0, 0
	.byte 'M', '-', '-', 0, 0, 0, 0, 0
	.byte 'N', '-', '.', 0, 0, 0, 0, 0
	.byte 'O', '-', '-', '-', 0, 0, 0, 0
	.byte 'P', '.', '-', '-', '.', 0, 0, 0
	.byte 'Q', '-', '-', '.', '-', 0, 0, 0
	.byte 'R', '.', '-', '.', 0, 0, 0, 0
	.byte 'S', '.', '.', '.', 0, 0, 0, 0
	.byte 'T', '-', 0, 0, 0, 0, 0, 0
	.byte 'U', '.', '.', '-', 0, 0, 0, 0
	.byte 'V', '.', '.', '.', '-', 0, 0, 0
	.byte 'W', '.', '-', '-', 0, 0, 0, 0
	.byte 'X', '-', '.', '.', '-', 0, 0, 0
	.byte 'Y', '-', '.', '-', '-', 0, 0, 0
	.byte 'Z', '-', '-', '.', '.', 0, 0, 0
	
message01:	.asciiz "A A A"
message02:	.asciiz "SOS"
message03:	.asciiz "WATERLOO"
message04:	.asciiz "DANCING QUEEN"
message05:	.asciiz "CHIQUITITA"
message06:	.asciiz "THE WINNER TAKES IT ALL"
message07:	.asciiz "MAMMA MIA"
message08:	.asciiz "TAKE A CHANCE ON ME"
message09:	.asciiz "KNOWING ME KNOWING YOU"
message10:	.asciiz "FERNANDO"

buffer01:	.space 128
buffer02:	.space 128
test_buffer:	.byte 0x30 0x37 0x30 0x00    # This is SOS
