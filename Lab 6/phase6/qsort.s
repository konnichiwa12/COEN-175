#function prologue:
readarray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$readarray.size, %esp
#block:
#assignment:
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L0:
	movl	-4(%ebp), %eax
	cmpl	n, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L1
#block:
#function call:
	movl	-4(%ebp), %eax
	imull	$4, %eax
	movl	a, %ecx
	addl	%eax, %ecx
	pushl	%ecx
	leal	.L2, %eax
	pushl	%eax
	call	scanf
	addl	$8, %esp
#assignment:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L0
.L1:
#function epilogue:

readarray.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	readarray.size, 4
	.globl	readarray

#function prologue:
writearray:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$writearray.size, %esp
#block:
#assignment:
	movl	$0, %eax
	movl	%eax, -4(%ebp)
.L3:
	movl	-4(%ebp), %eax
	cmpl	n, %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L4
#block:
#function call:
	movl	-4(%ebp), %eax
	imull	$4, %eax
	movl	a, %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	pushl	%ecx
	leal	.L5, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
#assignment:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L3
.L4:
#function call:
	leal	.L6, %eax
	pushl	%eax
	call	printf
	addl	$4, %esp
#function epilogue:

writearray.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	writearray.size, 4
	.globl	writearray

#function prologue:
exchange:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$exchange.size, %esp
#block:
#assignment:
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
#assignment:
	movl	12(%ebp), %eax
	movl	(%eax), %eax
	movl	8(%ebp), %ecx
	movl	%eax, (%ecx)
#assignment:
	movl	12(%ebp), %ecx
	movl	-4(%ebp), %edx
	movl	%edx, (%ecx)
#function epilogue:

exchange.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	exchange.size, 4
	.globl	exchange

#function prologue:
partition:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$partition.size, %esp
#block:
#assignment:
	movl	12(%ebp), %ecx
	imull	$4, %ecx
	movl	%eax, -16(%ebp)
	movl	8(%ebp), %eax
	addl	%ecx, %eax
	movl	(%eax), %eax
	movl	%eax, -12(%ebp)
#assignment:
	movl	12(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -4(%ebp)
#assignment:
	movl	16(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
.L7:
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L8
#block:
#assignment:
	movl	-8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -8(%ebp)
.L9:
	movl	-8(%ebp), %eax
	imull	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	cmpl	-12(%ebp), %ecx
	setg	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L10
#assignment:
	movl	-8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	.L9
.L10:
#assignment:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
.L11:
	movl	-4(%ebp), %eax
	imull	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	movl	(%ecx), %ecx
	cmpl	-12(%ebp), %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	$0, %ecx
	je	.L12
#assignment:
	movl	-4(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -4(%ebp)
	jmp	.L11
.L12:
#if statement
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L13
#function call:
	movl	-8(%ebp), %eax
	imull	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	pushl	%ecx
	movl	-4(%ebp), %eax
	imull	$4, %eax
	movl	8(%ebp), %ecx
	addl	%eax, %ecx
	pushl	%ecx
	movl	%edx, -20(%ebp)
	call	exchange
	addl	$8, %esp
.L13:
	jmp	.L7
.L8:
	movl	-8(%ebp), %eax
	jmp	partition.exit
#function epilogue:

partition.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	partition.size, 20
	.globl	partition

#function prologue:
quicksort:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$quicksort.size, %esp
#block:
#if statement
	movl	16(%ebp), %eax
	cmpl	12(%ebp), %eax
	setg	%al
	movzbl	%al, %eax
	cmpl	$0, %eax
	je	.L15
#block:
#assignment:
#function call:
	pushl	16(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	partition
	addl	$12, %esp
	movl	%eax, -4(%ebp)
#function call:
	pushl	-4(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	quicksort
	addl	$12, %esp
#function call:
	pushl	16(%ebp)
	movl	-4(%ebp), %eax
	addl	$1, %eax
	pushl	%eax
	pushl	8(%ebp)
	call	quicksort
	addl	$12, %esp
.L15:
#function epilogue:

quicksort.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	quicksort.size, 4
	.globl	quicksort

#function prologue:
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
#block:
#assignment:
	movl	$8, %eax
	movl	%eax, n
#assignment:
#function call:
	movl	n, %eax
	imull	$4, %eax
	pushl	%eax
	call	malloc
	addl	$4, %esp
	movl	%eax, a
#function call:
	call	readarray
#function call:
	movl	n, %eax
	subl	$1, %eax
	pushl	%eax
	pushl	$0
	pushl	a
	call	quicksort
	addl	$12, %esp
#function call:
	call	writearray
#function epilogue:

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 0
	.globl	main

	.comm	n, 4
	.comm	a, 4
	.data
.L6:	.asciz	"\012"
.L5:	.asciz	"%d "
.L2:	.asciz	"%d"
