# UVic CSC 230, Fall 2020
# Assignment #1, part A

# Student name:  Shanheng Chen
# Student number: V00946075


# Compute odd parity of word that must be in register $8
# Value of odd parity (0 or 1) must be in register $15


.text

start:
	lw $8, testcase1  # STUDENTS MAY MODIFY THE TESTCASE GIVEN IN THIS LINE
	
# STUDENTS MAY MODIFY CODE BELOW
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	li $9, 32
	li $10, 0x1	
	li $11, 0	#counter	
	
loop:
	beq $9, $0, endwhileloop
	and $12,  $8, $10
	beq $12, $0, firstbitqualzero
	addiu $11, $11, 1

firstbitqualzero:
	srl $8, $8, 1
	subiu $9, $9, 1
	j loop
	
	
endwhileloop:
	and $13, $11, $10 # odd parity if $13 = 0 then even = 1 then odd
	beq $13, $0, evenone
	addi $15, $0, 0
	j exit
	
evenone:
	addi $15, $0, 1
	beq $0, $0, exit


# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# STUDENTS MAY MODIFY CODE ABOVE


exit:
	add $2, $0, 10
	syscall
		

.data

testcase1:
	.word	0x00200020    # odd parity is 1

testcase2:
	.word 	0x00300020    # odd parity is 0
	
testcase3:
	.word  0x1234fedc     # odd parity is 0

