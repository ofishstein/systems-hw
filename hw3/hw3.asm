.globl main

.data
# char * [] data
dataAddr:.align 2
	.space 64
# char * data[] = {"names"};
array:
	.align 4
	.asciiz "Joe"
	.align 4
	.asciiz "Jenny"
	.align 4
	.asciiz "Jill"
	.align 4
	.asciiz "John"
	.align 4
	.asciiz "Jeff"
	.align 4
	.asciiz "Joyce"
	.align 4
	.asciiz "Jerry"
	.align 4
	.asciiz "Janice"
	.align 4
	.asciiz "Jake"
	.align 4
	.asciiz "Jonna"
	.align 4
	.asciiz "Jack"
	.align 4
	.asciiz "Jocelyn"
	.align 4
	.asciiz "Jessie"
	.align 4
	.asciiz "Jess"
	.align 4
	.asciiz "Janet"
	.align 4
	.asciiz "Jane"
	.align 4
# int size = 16
size: 	.align 4
	.word 16
# various strings used throughout print_array
initString: .asciiz "Initial array is:\n["
sortString: .asciiz "Insertion sort is finished!\n["
comma:	.asciiz ", "
endString: .asciiz "]\n"

.text
main:
	# char * [] data = { "names" }
	# initialize array
	la $t0, array
	la $t1, dataAddr
	li $t2, 0 #i = 0
	
	#initialize array contents
	init_array_contents_loop:
	beq $t2, 16, end_init
	#data[i] = &array[i]
	sw $t0, ($t1)
	#array = align 4 = 16 
	addi $t0, $t0, 16 
	#data = words = 4
	addi $t1, $t1, 4 
	#i++
	addi $t2, $t2, 1
	j init_array_contents_loop	

	end_init:
	#printf("Initial array is:\n");
	la $t0, initString
	move $a0, $t0
	li $v0, 4
	syscall
	
	#print_array(data, size);
	la $a0, dataAddr
	lw $a1, size
	jal print_array
	
	#insertSort(data, size);
	la $a0, dataAddr
	lw $a1, size
	jal insertSort

	#printf("Insertion sort is finished!\n");
	la $t0, sortString
	move $a0, $t0
	li $v0, 4
	syscall
	
	#print_array(data, size);
	la $a0, dataAddr
	lw $a1, size
	jal print_array
	
	#exit(0);
	li $v0, 10
	syscall
	
str_lt:
	# allocate stack
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	# char * x
	move $t0, $a0 
	# char * y
	move $t1, $a1 
	
	word_loop:
	#load 
	lb $t2, ($t0) 
	lb $t3, ($t1)
	
	and $t4, $t2, $t3
	# for (; *x!='\0' && *y!='\0'; x++, y++)
	beq $t4, $zero, str_end
	# if (x < y) 
	blt $t2, $t3, less_than 
	# if (y < x)
	bgt $t2, $t3, greater_than
	# x++ 
	addi $t0, $t0, 1
	# y++ 
	addi $t1, $t1, 1 
	j word_loop
	
	str_end:
	# if x == 0 return true
	beq $t2, $zero, less_than
	#else return false
	j greater_than 
	
	#return true
	less_than: 
	li $v0, 1
	j end_str_lt
	
	#return false
	greater_than: 
	li $v0, 0
	j end_str_lt
	
	end_str_lt:
	lw $ra, 0($sp)
	addi $sp, $sp 4
	
	jr $ra

insertSort:
	# allocate stack
	addi $sp, $sp, -24
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	sw $s3, 16($sp)
	sw $s4, 20($sp)

	# char *a[], size_t length
	move $s0, $a0
	move $s1, $a1
	# i
	li $s2, 1 
	
	array_loop:
	# for(i = 1; i < length;i++)
	beq $s2, $s1, end_loop
	
	#char *value = a[i];
	la $t0 ($s0)
	li $t1, 4
	# 4 * i
	mul $t2, $s2, $t1 
	# get address from data[i]
	add  $t3, $t0, $t2 
	# value = array[i]
	lw $s3, ($t3) 
	
	addi $s4, $s2, -1 #j = i-1
	
	compare_loop:
	# for (j = i-1; j >= 0 && str_lt(value, a[j]); j--)
	# j + 1 > 0 == j >=0
	addi $t0, $s4, 1 
	beq $t0, $zero, end_compare
	move $a0, $s3
	
	#str_lt(value, a[j]) == true
	la $t0, ($s0)
	li $t1, 4
	# 4 * j
	mul $t2, $s4, $t1 
	# get address from data[j]
	add $t3, $t0, $t2 
	# a[j] as argument
	lw $a1, ($t3) 
	
	jal str_lt
	move $t0, $v0
	# str_lt == true
	beq $t0, $zero, end_compare 
	addi $t1, $s4, 1
	# j >= 0
	beq $t1, $zero, end_compare
	
	la $t0, ($s0)
	li $t1, 4
	# 4 * j
	mul $t2, $s4, $t1
	# get address from data[j]
	add $t3, $t0, $t2 
	# $t4 = a[j] 
	lw $t4, ($t3) 
	
	move $t0, $s0
	li $t1, 4
	# j + 1
	addi $t2, $s4, 1
	# 4 * (j + 1)
	mul $t3, $t2, $t1 
	# get address from data
	add $t1, $t3, $t0 
	# a[j+1] = a[j]; a[j] == $t4
	sw $t4, ($t1) 
	
	#j--	
	addi $s4, $s4, -1
	# end for(j)
	j compare_loop 
	
	end_compare:
	move $t0, $s0
	li $t1, 4
	#j + 1
	addi $t2, $s4, 1
	# 4 * (j + 1) 
	mul $t4, $t2, $t1
	add $t1, $t4, $t0
	# a[j+1] = value;
	sw $s3, ($t1) 
	
	# i++
	addi $s2, $s2, 1
	# for(i) 
	j array_loop 
	
	end_loop:
	lw $s4, 20($sp)
	lw $s3, 16($sp)
	lw $s2, 12($sp)
	lw $s1, 8($sp)
	lw $s0, 4($sp)
	lw $ra, 0($sp)
	addi $sp, $sp, 24
	
	jr $ra

	
print_array:
	# allocate stack
	addi $sp, $sp -4
	sw $ra, 0($sp)
	
	move $t0, $a0
	# int i = size
	move $t1, $a1 
	
	print_loop:
	# while i > 0
	beq $t1, $zero, end_print 
	# printf( a[i] )
	lw $a0, ($t0) 
	li $v0, 4
	syscall
	
	addi $t0, $t0, 4
	addi $t1, $t1, -1

	beq $t1, 0, end_print
	la $t3, comma
	move $a0, $t3
	li $v0, 4
	syscall
	j print_loop

	end_print:
	la $t0, endString
	move $a0, $t0
	li $v0, 4
	syscall
	
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	
	jr $ra
	
