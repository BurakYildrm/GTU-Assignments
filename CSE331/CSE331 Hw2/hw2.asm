################# data fields #################
	.data 										# for data
fin:												# input file address
	.asciiz "input.txt"						
fout:												# output file address
	.asciiz "output.txt" 																		
cdd_sqnc:										# candidate sequence text
	.asciiz  "candidate sequence: ["
res_sqnc:										#result sequence text
	.asciiz "result sequence: ["		
comma:											# comma character
	.asciiz ", "								
closing_bracket:								# closing bracket character
	.asciiz "]"									
size:												# size text
	.asciiz " size: "						
newline:											# value to indicate the newlines in arrays (hexadecimal representation of the word 'newline' according to the alphabetical indexes of the letters)
	.word 0x075be4f5
empty:											# value for empty indexes of arrays (hexadecimal representation of the word 'empty' according to the alphabetical indexes of the letters)
	.word 0x002ec299
buffer:											# buffer to store the content of input file
	.space 1024										
numbers:											# array to store the numbers from input.txt
	.space 1024									
temp:												# array to store candidate sequences
	.space 1024									
result:											# array to store the result sequence
	.space 1024									
bigger_indexes:								# array to store the indexes whose content is bigger than current one's
	.space 1024
row_len:											# array to store the lengths of 6 rows inside numbers array
	.space 24							

############### main function ############### 
	.text 										# for execution
	.globl main									# main function is enabled globally
main:												# main functiom
	# opening the input file for reading
	li $v0, 13									# system call for open file
	la $a0, fin									# input file name
	li $a1, 0									# flag for reading
	li $a2, 0									# mode is ignored
	syscall 										# open the input file
	move $s0, $v0								# save the input file descriptor

	# reading from the input file
	li $v0, 14									# system call for read from file
	move $a0, $s0								# file descriptor
	la $a1, buffer								# address of buffer
	li $a2, 1024								# hardcoded buffer length
	syscall 										# read from the input file
	move $s1, $v0								# save the number of characters read to $s1
	
	# closing the input file
	li $v0, 16									# system call for close file
	move $a0, $s0								# file descriptor to close
	syscall										# close file
	
	# splitting the buffer
	la $a1, buffer								# load the address of buffer array to $a1
	la $a2, numbers							# load the address of numbers array to $a2
	move $a3, $s1								# load the number of characters read in read file part to $a3
	jal split									# split the buffer by comma delimiter
	move $s2, $v1								# save the meaningful length of the numbers array (including newline characters) (counter in c code)

	# finding the length of each row
	la $a1, numbers							# load address of numbers array to $a1
	move $a2, $s2								# load the meaningful length of numbers array to $a2
	la $a3, row_len							# load the address of row_len array to $a3
	jal findRowLength							# find the lengths of 6 row inside numbers array

	# clearing temp and result arrays
	la $a1, temp								# load address of temp array to $a1
	li $a2, 0									# load 0 (start index) to $a2 
	jal clearArray								# clear temp array
	la $a1, result								# load address of result array to $a1
	li $a2, 0									# load 0 (start index) to $a2
	jal clearArray								# clear result array
	
	# setting required variables
	la $s0, row_len							# address of row_len
	li $s1, 0									# index counter for bigger_indexes 				
	li $s3, 0									# index counter for numbers 															
	li $s4, 0									# index counter for temp 																
	li $s5, 0									# index counter for result 																
	lw $s6, row_len($zero)					# set the current row length to first integer in row_len	
	li $s7, 0									# set the current integer to 0	
	li $t0, 0									# set i to 0
	li $t1, 0									# set j to 0
	li $t2, 0									# set k to 0
	li $t3, 0									# set the result length to 0
	li $t4, 0									# set the temp length to 0
	li $t5, 0									# set number of bigger indexes to 0
	li $t6, 0									# temp variable 1
	li $t7, 0									# temp variable 2,
	li $t8, 0									# temp variable 3
	li $t9, 0									# temp variable 4
		
	# finding longest increasing sequence
	main_loop:
		beq $t0, $s2, part_after_m_loop	# if i == meaningful length of the numbers array, branch to the part after main loop
		bne $t0, $s6, find_sequences		# if i != current row length, branch to the part that finds sequences
		addi $s0, $s0, 4						# move to next integer in row_len
		lw $t6, 0($s0)							# load current integer in row_len to temp variable 1
		add $s6, $s6, $t6						# add the value in temp variable 1 to current row length
		addi $s6, $s6, 1						# increment current row length by 1 for that extra newline value at the end of the row
		jal printNewLine						# print new line
		li $v0, 4								# system call for print string
		la $a0, res_sqnc						# result sequence text
		syscall 									# print string
		la $a1, result						  	# load the address of result array to $a1
		jal findLength							# find the meaningful length of result array
		move $t6, $v1							# move the result to temp variable 1
		li $t1, 0								# j = 0
		li $s5, 0								# reset result index counter to 0
		print_res_loop:						# beginning of the print result loop
			beq $t1, $t6, print_res_size	# if j == meaningful length of result array, branch to print result size
			addi $t7, $t6, -1					# meaningful length of result array - 1
			lw $t8, result($s5)				# load the current result array element to temp variable 3
			li $v0, 1							# system call for print integer
			move $a0, $t8						# load integer to print to $a0
			syscall								# print integer
			bne $t1, $t7, print_w_comma	# if j != length - 1, branch to part that prints comma after current integer
			li $v0, 11							# system call for print character
			li $a0, ']'							# closing bracket character
			syscall								# print character
			j print_res_iter_end				# jump to the end of iteration
			print_w_comma:						# print ", "
			li $v0, 4							# system call for print string
			la $a0, comma						# address of comma
			syscall								# print string
			print_res_iter_end:				# end of iteration
			addi $t1, $t1, 1					# j++
			addi $s5, $s5, 4					# move to next integer location in result array
			j print_res_loop					# jump to the beginning of the loop
		print_res_size:						# print result length with size text in front of it
		li $v0, 4								# system call for print string
		la $a0, size							# address of size text
		syscall									# print string
		li $v0, 1								# system call for print integer
		move $a0, $t3							# meaningful length of result array
		syscall									# print integer
		jal printNewLine						# print new line
		jal printNewLine						# print new line
		la $a1, result							# address of result array (array to clear)
		li $a2, 0								# start index
		jal clearArray							# clear result array
		li $t3, 0								# reset result length to 0
		j main_loop_iter_end					# jump to the end of  main loop's current iteration
		find_sequences:						# find increasing sequences
		la $a1, bigger_indexes				# address of bigger_indexes array (array to clear)
		li $a2, 0								# start index
		jal clearArray							# clear bigger_indexes array
		la $a0, numbers						# address of numbers array
		move $a1, $s6							# current row length
		lw $a2, numbers($s3)					# current integer in numbers array
		move $a3, $t0							# start index
		jal findBiggerIndexes				# find indexes with bigger values than current integer in numbers array
		move $t5, $v1							# save the number of bigger indexes
		
		#li $v0, 1
		#lw $a0, numbers($s3)
		#syscall
		#jal printNewLine
		
		
		
		lw $t6, numbers($s3)					# load current integer in numbers array to temp variable 1
		sw $t6, temp($zero)					# save current integer in numbers array to first index of temp array
		li $t1, 0								# j = 0
		li $s1, 0								# reset index counter for bigger_indexes to 0
		find_sqnc_outer_loop:				# beginning of outer loop
			beq $t1, $t5, after_outer_l   # if j == number of bigger indexes, branch to after outer loop part
			la $a1, temp						# address of temp array (array to clear)
			li $a2, 1							# start index
			jal clearArray						# clear temp array
			lw $t6, bigger_indexes($s1)	# load the current integer from bigger_indexes array to temp variable 1
			li $t7, 4							# load 4 to temp variable 2
			mult $t6, $t7						# multiply the current integer from bigger_indexes by 4
			mflo $t6								# save the results to temp value 1
			lw $s7, numbers($t6)				# load the value at the index in temp variable 1 from numbers array to current integer
			sw $s7, temp($t7)					# load current integer to second index of temp array
			li $s4, 8							# load 8 to temp index counter (temp[2])
			lw $t2, bigger_indexes($s1)	# k = current integer from bigger_indexes array
			find_sqnc_inner_loop:			# beginning of the inner loop
				beq $t2, $s6, p_cdd_sqnc	# if k == current row length, branch to print candidate sequence part
				li $t7, 4						# load 4 to temp variable 2
				mult $t2, $t7					# multiply k by 4
				mflo $t7							# save the result to temp variable 2
				lw $t6, numbers($t7)			# save  numbers[k] to temp variable 1
				ble $t6, $s7, i_l_iter_end	# if numbers[k] <= current integer, branch to the end of inner loop's current iteration
				sw $t6, temp($s4)				# load number[k] to current temp array word location
				addi $s4, $s4, 4				# increment temp index counter by 4 to go to the next word location
				move $s7, $t6					# update current integer to numbers[k]
				i_l_iter_end:					# end of inner loop's current iteration 
				addi $t2, $t2, 1				# k++
				j find_sqnc_inner_loop		# jump to the beginning of the inner loop
			p_cdd_sqnc:							# print candidate sequence part
			la $a1, temp						# address of temp array
			jal findLength						# find the meaningful length of temp array
			move $t4, $v1						# save the results to temp length
			li $v0, 4							# system call for print string
			la $a0, cdd_sqnc					# candidate sequence text
			syscall								# print string
			li $t2, 0							# k = 0
			li $s4, 0							# reset temp index counter to 0
			print_cdd_loop:					# beginning of the print candidate loop
				beq $t2, $t4, p_cdd_size	# if k == temp length, branch to print candidate size
				lw $t6, temp($s4)				# load the current temp array element to temp variable 1
				li $v0, 1						# system call for print integer
				move $a0, $t6					# load integer to print to $a0
				syscall							# print integer
				addi $t6, $t4, -1				# temp length - 1
				bne $t2, $t6, p_w_comma		# if k != temp length - 1, branch to part that prints comma after current temp array element
				li $v0, 11						# system call for print character
				li $a0, ']'						# closing bracket character
				syscall							# print character					
				j print_cdd_iter_end			# jump to the end of print candidate loop's current iteration
				p_w_comma:						# print ", "
				li $v0, 4						# system call for print string
				la $a0, comma					# address of comma
				syscall							# print string
				print_cdd_iter_end:			# end of iteration
				addi $t2, $t2, 1				# k++
				addi $s4, $s4, 4				# move to next integer location in temp array
				j print_cdd_loop				# jump to the beginning of print candidate loop
			p_cdd_size:							# print temp length with size text in front of it
			li $v0, 4							# system call for print string
			la $a0, size						# size text
			syscall								# print string
			li $v0, 1							# system call for print integer
			move $a0, $t4						# meaningful length of temp array
			syscall								# print integer
			jal printNewLine					# print new line
			ble $t4, $t3, outer_l_iter_e  # if temp length <= result length, branct to the end of find sequence outer loop's current iteration
			move $t3, $t4						# result lenght = temp length
			la $a1, temp						# address of temp array
			la $a2, result						# address of result array
			jal copyArray						# copy temp array to result array
			outer_l_iter_e:					# end of outer loop's current iteration
			addi $t1, $t1, 1					# j++
			addi $s1, $s1, 4					# move to next integer in bigger_indexes array
			j find_sqnc_outer_loop			# jump to the beginning of the  		
		after_outer_l:							# after outer loop part
		bne $t5, $zero, check_row_len		# if number of bigger indexes is not 0, branch to check row length part
		li $v0, 4								# system call for print string
		la $a0, cdd_sqnc						# candidate sequence text
		syscall									# print string
		li $v0, 1								# system call for print integer
		lw $a0, temp($zero)					# first integer in temp array
		syscall									# print integer
		li $v0, 11								# system call for print character
		li $a0, ']'								# closing bracket character
		syscall									# print character
		li $v0, 4								# system call for print string
		la $a0, size							# size text
		syscall									# print string
		li $v0, 1								# system call for print integer
		li $a0, 1								# integer to print
		syscall 									# print integer
		jal printNewLine						# print new line
		check_row_len:							# check if the length of current row is 1
		lw $t6, 0($s0)							# load length of current row to temp variable 1
		li $t7, 1								# load 1 to temp variable 2
		bne $t6, $t7, main_loop_iter_end # if length of current row is not 1, branch to the end of main loop's current iteration
		li $t3, 1								# set result length to 1
		la $a1, temp							# address of temp array
		la $a2, result							# address of result array
		jal copyArray							# copy temp array to result array
		la $a1, temp							# address of temp array
		li $a2, 0								# start index
		jal clearArray							# clear temp array
		main_loop_iter_end:					# end of main loop's current iteration
		addi $t0, $t0, 1						# i++
		addi $s3, $s3, 4						# move to next integer in numbers array
		j main_loop								# jump to the beginning of main loop
	
	# print last result sequence
	part_after_m_loop:						# part after main loop
	jal printNewLine							# print new line
	li $v0, 4									# system call for print string
	la $a0, res_sqnc							# result sequence text
	syscall 										# print string
	la $a1, result						  		# load the address of result array to $a1
	jal findLength								# find the meaningful length of result array
	move $t6, $v1								# move the result to temp variable 1
	li $t1, 0									# j = 0
	li $s5, 0									# reset result index counter to 0
	print_last_res_loop:						# beginning of the print last result loop
		beq $t1, $t6, p_last_res_size		# if j == meaningful length of result array, branch to print last result size
		addi $t7, $t6, -1						# meaningful length of result array - 1
		lw $t8, result($s5)					# load the current result array element to temp variable 3
		li $v0, 1								# system call for print integer
		move $a0, $t8							# load integer to print to $a0
		syscall									# print integer
		bne $t1, $t7, print_w_comma_last	# if j != length - 1, branch to part that prints comma after current integer
		li $v0, 11								# system call for print character
		li $a0, ']'								# closing bracket character
		syscall									# print character
		j print_last_res_iter_end			# jump to the end of iteration
		print_w_comma_last:					# print ", "
		li $v0, 4								# system call for print string
		la $a0, comma							# address of comma
		syscall									# print string
		print_last_res_iter_end:			# end of iteration
		addi $t1, $t1, 1						# j++
		addi $s5, $s5, 4						# move to next integer location in result array
		j print_last_res_loop				# jump to the beginning of the print last result loop
	p_last_res_size:							# print last result length with size text in front of it
	li $v0, 4									# system call for print string
	la $a0, size								# address of size text
	syscall										# print string
	li $v0, 1									# system call for print integer
	move $a0, $t3								# meaningful length of result array
	syscall										# print integer
	
	# exit
	li $v0, 10									# system call for exit
	syscall 										# exit
	
################### functions ################### 
	.text											# for execution
printNewLine:									# prints new line
	li $v0, 11									# system call for print character
	li $a0, '\n'								# load new line character to $a0
	syscall										# print new line
	jr $ra										# return
	
split:											# splits the given string according to the comma delimiter ($a1 => source, $a2 => destination, $a3 => source length)
	li $t0, ','									# load comma character to $t0
	li $t1, 0									# iteration count set to 0
	li $t2, 0									# destination index set to 0
	li $t3, 0									# current character set to 0
	li $t4, 0									# current word set to 0
	li $t5, 10									# load 10 to $t5
	li $t6, 0									# temp value set to 0
	li $t7, 0									# next character set to 0
	
	split_loop_begin:							# beginning of the loop
		beq $t1, $a3, finish_split			# if iteration count is equal to source length, break loop
		lb $t3, 0($a1)							# load the current byte to $t3
		li $t6, '\n'							# load newline character to $t6
		bne $t3, $t6, check_carriage		# if current character is not newline character, branch to check_carriage (for both windows and unix-based systems) (if-else if statement)
		lw $t4, newline						# load the load the newline value to $t4
		sw $t4, 0($a2)							# change the content of current position with $t4
		addi $a2, $a2, 4						# move to the new word
		addi $t2, $t2, 1						# increment destination index by 1
		j split_iter_end						# jump to split_iter_end
		check_carriage:						# check for carriage return character 
		li $t6, '\r'							# load carriage return character to $t6
		bne $t3, $t6, check_comma			# if current character is not carriage return, branch to check_comma
		lw $t4, newline						# load the newline value to $t4
		sw $t4, 0($a2)							# change the content of current position with $t4
		addi $a2, $a2, 4						# move to the new word
		addi $t2, $t2, 1						# increment destination index by 1
		addi $a1, $a1, 1						# skip newline character
		addi $t1, $t1, 1						# increment the iteration count by 1
		j split_iter_end						# jump to split_iter_end
		check_comma:							# check for comma character
		beq $t3, $t0, split_iter_end		# if current character is comma, skip this part
		lw $t4, 0($a2)							# load the current word of destination to $t4
		mult $t4, $t5							# multiply the current word by 10
		mflo $t4									# load the result to $t4
		addi $t3, $t3, -48					# subtract 48 to convert $t3 from character to integer
		add $t4, $t4, $t3						# calculate the new word
		sw $t4, 0($a2)							# update current index of the array
		addi $a2, $a2, 4						# move to the new word
		addi $t2, $t2, 1						# increment destination index by 1
		move $t6, $t1							# load iteration count to $t6
		addi $t6, $t6, 1						# increment $t6 by 1
		beq $t6, $a3, split_iter_end		# if iteration counter + 1 = source length, branch to split_iter_end
		lb $t7, 1($a1)							# load next character to $t7
		beq $t7, $t0, split_iter_end		# if next character is comma, branch to split_iter_end
		li $t6, '\n'							# load new line character to $t6
		beq $t7, $t6, split_iter_end		# if next character is newline, branch to split_iter_end
		li $t6, '\r'							# load new line character to $t6
		beq $t7, $t6, split_iter_end		# if next character is carriage return, branch to split_iter_end
		addi $a2, $a2, -4						# decrease destination by 1 word
		addi $t2, $t2, -1						# decrement destination index by 1
		split_iter_end:						# excute at each iteration no matter what
		addi $t1, $t1, 1						# increment the iteration count by 1
		addi $a1, $a1, 1						# move to next location
		j split_loop_begin					# jump to the beginning of the loop
	finish_split:								# final executions of the function
	move $v1, $t2								# move the number of integers read (including newline characters) to $v1
	jr $ra										# return
	
clearArray:										# clears the given array starting from the given index ($a1 => array, $a2 => start index)
	li $t6, 0									# iteration count set to 0
	li $t7, 4									# load 4 to $t7
	add $t6, $t6, $a2							# iteration count start with start index
	mult $a2, $t7								# multiply start index by 4
	mflo $a2										# put the result to $a2
	add $a1, $a1, $a2							# move to the given index
	lw $t7, empty								# load empty index value to $t7
	
	clear:										# beginning of the loop
		beq $t6, 256, finish_clear			# if the iteration count == 1024, break loop
		sw $t7, 0($a1)							# save empty index value to current array location
		addi $a1, $a1, 4						# move to next loaciton
		addi $t6, $t6, 1						# increment iteration count by 1
		j clear									# jump to the beginning of the loop
	finish_clear:								# finish function
	jr $ra										# return			
	
findBiggerIndexes:							# finds indexes with bigger values than given value starting from given index ($a0 => source, $a1 => length, $a2 => target value, $a3 => start index)
	li $t6, 0									# iteration count set to 0
	la $t7, bigger_indexes					# load the address of bigger_indexes array (destination array) to $t7
	li $t8, 0									# number of bigger indexes set to 0
	li $t9, 4									# load 4 to $t9 (temp)
	add $t6, $t6, $a3							# iteration count begin with given index
	mult $a3, $t9								# multiply start index by 4
	mflo $a3										# put the result to $a3
	add $a0, $a0, $a3							# move to the given index
	
	find_index_loop:							# beginning of the loop
		beq $t6, $a1, finish_find			# if iteration count == length, branch to finish_loop
		lw $t9, 0($a0)							# load the integer at current location to $t9
		ble $t9, $a2, find_iter_end		# if the integer at current location is less than or equal to target value, branch to find_iter_end
		sw $t6, 0($t7)							# load the current index to current bigger_index location
		addi $t7, $t7, 4						# move to next location
		addi $t8, $t8, 1						# increment the number of bigger indexes by 1
		find_iter_end:							# execute at the end of each iteration no matter what
		addi $t6, $t6, 1						# increment iteration count by 1
		addi $a0, $a0, 4						# move to next integer
		j find_index_loop						# jump to find_index_loop
	finish_find:								# finish this function
	move $v1, $t8								# move the number of bigger indexes to $v1
	jr $ra										# return
	
copyArray:										# copies the content of source array to target array ($a1 => source, $a2 => target)
	move $t8, $ra								# save the value of $ra to $t0
	move $t9, $a1								# save the address of source array to $t1
	move $a1, $a2								# move the address of target array to $a1 to use it in clearArray function
	addi $sp, $sp, -4							# move stack pointer by 4 bytes to store a word
	sw $a2, 0($sp)								# store target address to current location of stack pointer
	li $a2, 0									# load 0 to $a2 to use it in clearArray function
	jal clearArray								# call clearArray function
	lw $a2, 0($sp)								# restore the address of target array ($a2) from stack pointer
	addi $sp, $sp, 4							# go back to the original location of stack pointer
	move $a1, $t9								# restore the address of source array ($a1) from $t1
	move $ra, $t8								# restore the value of $ra from $t0
	
	li $t8, 0									# iteration count set to 0
	li $t9, 0									# current integer set to 0
	
	copy_loop:									# beginning of the loop
		beq $t8, 256, finish_copy			# if iteration count == 1024, branch to finish_copy
		lw $t9, 0($a1)							# load the integer at the current source location
		sw $t9, 0($a2)							# save the integer at the current source location to current target location
		addi $a1, $a1, 4						# move to next location
		addi $a2, $a2, 4						# move to next location
		addi $t8, $t8, 1						# increment iteration count by 1
		j copy_loop								# jump to the beginning of loop
	finish_copy:								# finish copy
	jr $ra										# return

findLength:										# finds the meaningful length of the given array ($a1 => array)
	li $t6, 0									# set iteration count (meaningful length) to 0
	lw $t7, empty								# load empty index value to $t7
	li $t8, 0									# set current integer to 0
	
	find_len_loop:								# beginning of loop
		beq $t6, 256, finish_find_len	# if i == 1024, branch to the end of function
		lw $t8, 0($a1)							# load the current integer to $t2
		beq $t8, $t7, finish_find_len		# if the current integer is empty index value, branch to the end of function
		addi $t6, $t6, 1						# increment iteration count by 1
		addi $a1, $a1, 4
		j find_len_loop						# jump to the beginning of loop
	finish_find_len:							# end of function
	move $v1, $t6								# save the meaningful length of array to $v1
	jr $ra										# return
		
findRowLength:									# calculates the length of each row in source array and saves the results to destination array ($a1 => source, $a2 => source meaningful length, $a3 => destination)
	li $t0, 0									# set i to 0
	li $t1, 0									# set row length counter to 0
	li $t2, 0									# current integer
	lw $t3, newline							# newline value
	
	find_r_len_loop:							# beginning of loop
		beq $t0, $a2, finish_find_r_len	# if i == length, branch to the end of function
		lw $t2, 0($a1)							# load current integer to $t2
		bne $t2, $t3, inc_r_count			# if current integer is not newline value, branch to increment row counter part
		sw $t1, 0($a3)							# save the row length counter to current location of destination array
		addi $a3, $a3, 4						# move to next location
		li $t1, 0								# reset row length counter to 0
		j find_r_len_iter_end				# jump to the end of loop
		inc_r_count:							# increment row length counter part
		addi $t1, $t1, 1						# increment row length counter by 1
		find_r_len_iter_end:					# end of iteration
		addi $t0, $t0, 1						# increment i by 1
		addi $a1, $a1, 4						# move to next integer
		j find_r_len_loop						# jump to the beginning ýf loop
	finish_find_r_len:						# end of function
	sw $t1, 0($a3)								# save row length counter to current location of destination array
	jr $ra										# return
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
