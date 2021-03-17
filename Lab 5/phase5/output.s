towers:
#function prologue:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0, %esp
#block:
#function call:
	pushl	16(%ebp)
	pushl	20(%ebp)
	pushl	12(%ebp)
	pushl	8(%ebp)
	call	call_towers
#function call:
	pushl	16(%ebp)
	pushl	12(%ebp)
	call	print_move
#function call:
	pushl	12(%ebp)
	pushl	16(%ebp)
	pushl	20(%ebp)
	pushl	8(%ebp)
	call	call_towers
#function epilogue:
	movl	%ebp, %esp
	popl	%ebp
	ret
	.globl	towers
main:
#function prologue:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
#block:
#assignment:
	movl	$3, %eax
	movl	%eax, -4(%ebp)
#function call:
	pushl	-4(%ebp)
	call	print
#function call:
	pushl	$3
	pushl	$2
	pushl	$1
	pushl	-4(%ebp)
	call	towers
#function epilogue:
	movl	%ebp, %esp
	popl	%ebp
	ret
	.globl	main
#global variables:
