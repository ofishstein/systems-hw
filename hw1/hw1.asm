# program based off of hw1-factorial.c 
# computes the factorial of an input number
.globl main
.data
	# set up strings
	prmpt_msg: .word prmpt_msg_data		
	resp1_msg: .word resp1_msg_data
  	resp2_msg: .word resp2_msg_data
  	eol: .word eol_data
	# set up data for strings
  	prmpt_msg_data: .asciiz "Positive integer: "
  	resp1_msg_data: .asciiz "The value of 'factorial("
  	resp2_msg_data: .asciiz ")' is: "
  	eol_data: .asciiz "\n"  
.text
main:
	# return addr is  0($sp)
	# input number to 4($sp)
	# return values to 8($sp)	
	# printf("Positive integer: ");
	la $t0, prmpt_msg		# loads address of prmpt_msg into $t0
	li $v0, 4			# loads system call for print_str
	lw $a0, 0($t0)			# loads data from $t0 into $a0 for printing
	syscall				# equivalent to print_str
	# scanf("%d", &number);
	li $v0, 5			# loads system call for read_int 
	syscall 			# equivalent to read_int
	move $t0, $v0			# stores input in $t0
	# set up and store in stack
	subi $sp, $sp, 16		# reserves stack memory
	sw $ra, 0($sp)			# store return address in stack
	sw $t0, 4($sp)			# stores input in stack
	# factorial(number)
	jal factorial
	# return from previous call
	lw $s0, 8($sp)			# load final return value to $s0
	# printf("The value of 'factorial(%d)' is:  %d\n", number, factorial(number));
	# "The value of 'factorial(
	la $t0, resp1_msg		# loads address of resp1_msg into $t0
	li $v0, 4			# loads system call for print_str
	lw $a0, 0($t0)			# loads data from $t0 into $a0 for printing
	syscall				# equivalent to print_str
	# %d = number
	lw $t0, 4($sp)			# restore input to $t0
	li $v0, 1			# load system call for print_int
	move $a0, $t0			# move input from $t0 to $a0 for printint
	syscall				# equivalent to print_int
	# )' is: 
	la $t0, resp2_msg		# loads address of resp2_msg into $t0
	li $v0, 4			# loads system call for print_str
	lw $a0, 0($t0)			# loads data from $t0 into $a0 for printing
	syscall				# equivalent to print_str
	# %d = factorial(number)
	li $v0, 1			# load system call for print_int
	move $a0, $s0			# move result from $s0 to $a0 for printint
	syscall				# equivalent to print_int
	# \n"
	la $t0, eol			# loads address of eol into $t0
	li $v0, 4			# loads system call for print_str
	lw $a0, 0($t0)			# loads data from $t0 into $a0 for printing
	syscall				# equivalent to print_str
	# restore stack before exit
	lw $ra, 0($sp)			# restore return address from stack
	addi $sp, $sp, 16		# free up stack memory
	# return 0;
  	li $v0, 10         		# loads system call for exit
  	li $a0, 0			# sets return code to 0 (success)
  	syscall				# equivalent to exit
  	
factorial:
	# load input from parent stack
	lw $t0, 4($sp)			# loads input from stack into $t0
	# if (x == 0)
	beq $t0, $zero, return_one 	# if $t0 equals zero go to return_one
	# else
	# (x - 1)
	subi $t0, $t0, 1		# subtracts 1 from $t0 if not equal to 0
	# set up local stack for call 
	subi $sp, $sp, 16 		# move up stack pointer for recursion
	# factorial(x-1)
	sw $t0, 4($sp)       		# store working number in stack
  	sw $ra, 0($sp)			# store return address in stack
  	jal factorial 			# calls factorial recursively
	# return from previous call
	lw $ra, 0($sp) 			# restore the call's $ra 
	lw $t1, 8($sp)			# load the result from the child call to $t1
	addi $sp, $sp, 16		# move stack back to parent call
	lw $t2, 4($sp)			# load parent call's start value to $t2
	# return x * factorial(x-1);
	mul $t3, $t2, $t1 		# multiply parent's working value by child's return value and store in $t3
	sw $t3, 8($sp)			# store result from $t3 in parent's return value
	jr $ra				# return to parent call
	
return_one: 
	# return 1;
	li $t0, 1			# load 1 to $t0 
	sw $t0, 8($sp)			# store $t0 in parent call's stack
	jr $ra				# return to parent call
