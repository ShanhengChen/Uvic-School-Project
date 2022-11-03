# UVic CSC 230, Fall 2020
# Assignment #1, part B

# Student name: Shanheng Chen
# Student number: V00946075


# Compute the reverse of the input bit sequence that must be stored
# in register $8, and the reverse must be in register $15.


.text
start:
	lw $8, testcase3  # STUDENTS MAY MODIFY THE TESTCASE GIVEN IN THIS LINE
	
# STUDENTS MAY MODIFY CODE BELOW
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	li $9, 32 
	li $10, 0x1	
	li $11, 0x80000000 
	li $12, 0x00000000
loop:
	beq $9, $0, endwhileloop
	and $13, $8, $10
	beq $13, $0, skipbit
	or $12, $12, $11
	
skipbit:
	srl $11,$11,1
	srl $8,$8,1
	subiu $9, $9, 1
	j loop
	
	
endwhileloop:
	add $15, $15, $12
	beq $0, $0, exit

# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# STUDENTS MAY MODIFY CODE ABOVE

exit:
	add $2, $0, 10
	syscall
	
	

.data

testcase1:
	.word	0x00200020    # reverse is 0x04000400

testcase2:
	.word 	0x00300020    # reverse is 0x04000c00
	
testcase3:
	.word	0x1234fedc     # reverse is 0x3b7f2c48
