# UVic CSC 230, Fall 2020
# Assignment #1, part C

# Student name:  Shanheng Chen
# Student number: V00946075


# Compute M / N, where M must be in $8, N must be in $9,
# and M / N must be in $15.
# N will never be 0


.text
start:
	lw $8, testcase1_M
	lw $9, testcase1_N

# STUDENTS MAY MODIFY CODE BELOW
# vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	addiu $10, $0, 0
loop:
	bltu $8, $9, end
	sub $8, $8, $9
	addiu $10, $10, 1	
	j loop
	
end:	
	add $15, $0, $10

# ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
# STUDENTS MAY MODIFY CODE ABOVE

exit:
	add $2, $0, 10
	syscall
		

.data

# testcase1: 370 / 120 = 3
#
testcase1_M:
	.word	370
testcase1_N:
	.word 	120
	
# testcase2: 24156 / 77 = 313
#
testcase2_M:
	.word	24156
testcase2_N:
	.word 	77
	
# testcase3: 33 / 120 = 0
#
testcase3_M:
	.word	33
testcase3_N:
	.word 	120
