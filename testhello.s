	.file	"testhello.c"
	.section	.rodata
.LC0:
	.string	"myfunction_for_test!!! "
	.text
.globl myfunction_for_test
	.type	myfunction_for_test, @function
myfunction_for_test:
.LFB0:
	pushq	%rbp
.LCFI0:
	movq	%rsp, %rbp
.LCFI1:
	subq	$16, %rsp
.LCFI2:
	movsd	%xmm0, -8(%rbp)
	movl	$.LC0, %edi
	call	puts
	movsd	.LC1(%rip), %xmm0
	ucomisd	-8(%rbp), %xmm0
	seta	%al
	testb	%al, %al
	je	.L2
	movl	$0, %eax
	jmp	.L3
.L2:
	movl	$1, %eax
.L3:
	leave
.LCFI3:
	ret
.LFE0:
	.size	myfunction_for_test, .-myfunction_for_test
.globl testhello
	.type	testhello, @function
testhello:
.LFB1:
	pushq	%rbp
.LCFI4:
	movq	%rsp, %rbp
.LCFI5:
	subq	$48, %rsp
.LCFI6:
	movsd	.LC2(%rip), %xmm0
	movl	$-256, 40(%rsp)
	movl	$5, 32(%rsp)
	movl	$201, 24(%rsp)
	movl	$4, 16(%rsp)
	movl	$201, 8(%rsp)
	movl	$1, (%rsp)
	movl	$200, %r9d
	movl	$0, %r8d
	movl	$27, %ecx
	movl	$0, %edx
	movl	$0, %esi
	movl	$0, %edi
	movl	$1, %eax
	call	expect_func
	movsd	.LC3(%rip), %xmm0
	movl	$-256, %r9d
	movapd	%xmm0, %xmm1
	movl	$201, %r8d
	movl	$1, %ecx
	movl	$200, %edx
	movl	$myfunction_for_test, %esi
	xorpd	%xmm0, %xmm0
	movl	$3, %edi
	movl	$2, %eax
	call	expect_foo
	movl	$3, %esi
	movl	$2, %edi
	call	function
	leave
.LCFI7:
	ret
.LFE1:
	.size	testhello, .-testhello
.globl main
	.type	main, @function
main:
.LFB2:
	pushq	%rbp
.LCFI8:
	movq	%rsp, %rbp
.LCFI9:
	subq	$16, %rsp
.LCFI10:
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, %eax
	call	testhello
	movl	$1, %eax
	leave
.LCFI11:
	ret
.LFE2:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1075707904
	.align 8
.LC2:
	.long	2576980378
	.long	1075157401
	.align 8
.LC3:
	.long	1717986918
	.long	1075209830
	.section	.eh_frame,"a",@progbits
.Lframe1:
	.long	.LECIE1-.LSCIE1
.LSCIE1:
	.long	0x0
	.byte	0x1
	.string	"zR"
	.uleb128 0x1
	.sleb128 -8
	.byte	0x10
	.uleb128 0x1
	.byte	0x3
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.byte	0x90
	.uleb128 0x1
	.align 8
.LECIE1:
.LSFDE1:
	.long	.LEFDE1-.LASFDE1
.LASFDE1:
	.long	.LASFDE1-.Lframe1
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI0-.LFB0
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI3-.LCFI1
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.align 8
.LEFDE1:
.LSFDE3:
	.long	.LEFDE3-.LASFDE3
.LASFDE3:
	.long	.LASFDE3-.Lframe1
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI4-.LFB1
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI5-.LCFI4
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI7-.LCFI5
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.align 8
.LEFDE3:
.LSFDE5:
	.long	.LEFDE5-.LASFDE5
.LASFDE5:
	.long	.LASFDE5-.Lframe1
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x0
	.byte	0x4
	.long	.LCFI8-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x86
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI9-.LCFI8
	.byte	0xd
	.uleb128 0x6
	.byte	0x4
	.long	.LCFI11-.LCFI9
	.byte	0xc
	.uleb128 0x7
	.uleb128 0x8
	.align 8
.LEFDE5:
	.ident	"GCC: (GNU) 4.5.1"
	.section	.note.GNU-stack,"",@progbits
