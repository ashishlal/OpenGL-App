	.mod_init_func
	.align 3
	.quad	__GLOBAL__I__ZN4Jpeg4destE
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.align 1
.globl __ZN7ICCJpegC2Ev
	.weak_definition __ZN7ICCJpegC2Ev
__ZN7ICCJpegC2Ev:
LFB13:
	pushq	%rbp
LCFI0:
	movq	%rsp, %rbp
LCFI1:
	movq	%rdi, -8(%rbp)
	movq	__ZTV7ICCJpeg@GOTPCREL(%rip), %rax
	leaq	16(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	leave
	ret
LFE13:
	.align 1
.globl __ZN4Jpeg6CCJpegC1Ev
	.weak_definition __ZN4Jpeg6CCJpegC1Ev
__ZN4Jpeg6CCJpegC1Ev:
LFB16:
	pushq	%rbp
LCFI2:
	movq	%rsp, %rbp
LCFI3:
	subq	$16, %rsp
LCFI4:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rdi
	call	__ZN7ICCJpegC2Ev
	leaq	16+__ZTVN4Jpeg6CCJpegE(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	leave
	ret
LFE16:
	.text
.globl __ZN4Jpeg16init_destinationEP20jpeg_compress_struct
__ZN4Jpeg16init_destinationEP20jpeg_compress_struct:
LFB1497:
	pushq	%rbp
LCFI5:
	movq	%rsp, %rbp
LCFI6:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, __ZN4Jpeg4destE(%rip)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	40(%rax), %rax
	movq	%rax, (%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	48(%rax), %rax
	movq	%rax, 8(%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	$0, 56(%rax)
	leave
	ret
LFE1497:
.globl __ZN4Jpeg19empty_output_bufferEP20jpeg_compress_struct
__ZN4Jpeg19empty_output_bufferEP20jpeg_compress_struct:
LFB1498:
	pushq	%rbp
LCFI7:
	movq	%rsp, %rbp
LCFI8:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, __ZN4Jpeg4destE(%rip)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	40(%rax), %rax
	movq	%rax, (%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	48(%rax), %rax
	movq	%rax, 8(%rdx)
	movl	$0, %eax
	leave
	ret
LFE1498:
.globl __ZN4Jpeg16term_destinationEP20jpeg_compress_struct
__ZN4Jpeg16term_destinationEP20jpeg_compress_struct:
LFB1499:
	pushq	%rbp
LCFI9:
	movq	%rsp, %rbp
LCFI10:
	pushq	%rbx
LCFI11:
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, __ZN4Jpeg4destE(%rip)
	movq	__ZN4Jpeg4destE(%rip), %rcx
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	48(%rax), %rdx
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	8(%rax), %rax
	movq	%rdx, %rbx
	subq	%rax, %rbx
	movq	%rbx, %rax
	movq	%rax, 56(%rcx)
	popq	%rbx
	leave
	ret
LFE1499:
	.align 1,0x90
.globl __ZN4Jpeg6CCJpeg13jpeg_mem_destEP20jpeg_compress_structPhm
__ZN4Jpeg6CCJpeg13jpeg_mem_destEP20jpeg_compress_structPhm:
LFB1502:
	pushq	%rbp
LCFI12:
	movq	%rsp, %rbp
LCFI13:
	subq	$32, %rsp
LCFI14:
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	testq	%rax, %rax
	jne	L12
	movq	-8(%rbp), %rax
	movq	8(%rax), %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdi
	movl	$64, %edx
	movl	$0, %esi
	call	*%rax
	movq	%rax, %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, 40(%rax)
L12:
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, __ZN4Jpeg4destE(%rip)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	leaq	__ZN4Jpeg16init_destinationEP20jpeg_compress_struct(%rip), %rax
	movq	%rax, 16(%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	leaq	__ZN4Jpeg19empty_output_bufferEP20jpeg_compress_struct(%rip), %rax
	movq	%rax, 24(%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	leaq	__ZN4Jpeg16term_destinationEP20jpeg_compress_struct(%rip), %rax
	movq	%rax, 32(%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	movq	-16(%rbp), %rax
	movq	%rax, 40(%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rdx
	movq	-24(%rbp), %rax
	movq	%rax, 48(%rdx)
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	$0, 56(%rax)
	leave
	ret
LFE1502:
	.align 1,0x90
.globl __ZN4Jpeg6CCJpeg13jpeg_mem_sizeEP20jpeg_compress_struct
__ZN4Jpeg6CCJpeg13jpeg_mem_sizeEP20jpeg_compress_struct:
LFB1503:
	pushq	%rbp
LCFI15:
	movq	%rsp, %rbp
LCFI16:
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	40(%rax), %rax
	movq	%rax, __ZN4Jpeg4destE(%rip)
	movq	__ZN4Jpeg4destE(%rip), %rax
	movq	56(%rax), %rax
	leave
	ret
LFE1503:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
.globl __ZSt3minImERKT_S2_S2_
	.weak_definition __ZSt3minImERKT_S2_S2_
__ZSt3minImERKT_S2_S2_:
LFB1504:
	pushq	%rbp
LCFI17:
	movq	%rsp, %rbp
LCFI18:
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax
	movq	(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	cmpq	%rax, %rdx
	jae	L18
	movq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)
	jmp	L20
L18:
	movq	-8(%rbp), %rax
	movq	%rax, -24(%rbp)
L20:
	movq	-24(%rbp), %rax
	leave
	ret
LFE1504:
	.text
__ZStL17__verify_groupingPKcmRKSs:
LFB1425:
	pushq	%rbp
LCFI19:
	movq	%rsp, %rbp
LCFI20:
	pushq	%rbx
LCFI21:
	subq	$88, %rsp
LCFI22:
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%rdx, -88(%rbp)
	movq	-88(%rbp), %rdi
	call	__ZNKSs4sizeEv
	decq	%rax
	movq	%rax, -32(%rbp)
	movq	-80(%rbp), %rax
	decq	%rax
	movq	%rax, -48(%rbp)
	leaq	-48(%rbp), %rsi
	leaq	-32(%rbp), %rdi
	call	__ZSt3minImERKT_S2_S2_
	movq	(%rax), %rax
	movq	%rax, -40(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, -56(%rbp)
	movb	$1, -17(%rbp)
	movq	$0, -64(%rbp)
	jmp	L23
L24:
	movq	-56(%rbp), %rsi
	movq	-88(%rbp), %rdi
	call	__ZNKSsixEm
	movzbl	(%rax), %edx
	movq	-64(%rbp), %rax
	addq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	%al, %dl
	sete	%al
	movb	%al, -17(%rbp)
	decq	-56(%rbp)
	incq	-64(%rbp)
L23:
	movq	-64(%rbp), %rax
	cmpq	-40(%rbp), %rax
	jae	L27
	cmpb	$0, -17(%rbp)
	jne	L24
	jmp	L27
L28:
	movq	-56(%rbp), %rsi
	movq	-88(%rbp), %rdi
	call	__ZNKSsixEm
	movzbl	(%rax), %edx
	movq	-40(%rbp), %rax
	addq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	%al, %dl
	sete	%al
	movb	%al, -17(%rbp)
	decq	-56(%rbp)
L27:
	cmpq	$0, -56(%rbp)
	je	L29
	cmpb	$0, -17(%rbp)
	jne	L28
L29:
	movq	-40(%rbp), %rax
	addq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jle	L31
	movzbl	-17(%rbp), %ebx
	movq	-88(%rbp), %rdi
	movl	$0, %esi
	call	__ZNKSsixEm
	movzbl	(%rax), %edx
	movq	-40(%rbp), %rax
	addq	-72(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	%al, %dl
	setle	%al
	movzbl	%al, %eax
	andl	%ebx, %eax
	testl	%eax, %eax
	setne	%al
	movb	%al, -17(%rbp)
L31:
	movzbl	-17(%rbp), %eax
	addq	$88, %rsp
	popq	%rbx
	leave
	ret
LFE1425:
	.section __TEXT,__textcoal_nt,coalesced,pure_instructions
	.align 1
.globl __ZN4Jpeg6CCJpegD0Ev
	.weak_definition __ZN4Jpeg6CCJpegD0Ev
__ZN4Jpeg6CCJpegD0Ev:
LFB20:
	pushq	%rbp
LCFI23:
	movq	%rsp, %rbp
LCFI24:
	subq	$16, %rsp
LCFI25:
	movq	%rdi, -8(%rbp)
	leaq	16+__ZTVN4Jpeg6CCJpegE(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$1, %eax
	testb	%al, %al
	je	L38
	movq	-8(%rbp), %rdi
	call	__ZdlPv
L38:
	leave
	ret
LFE20:
	.align 1
.globl __ZN4Jpeg6CCJpegD1Ev
	.weak_definition __ZN4Jpeg6CCJpegD1Ev
__ZN4Jpeg6CCJpegD1Ev:
LFB19:
	pushq	%rbp
LCFI26:
	movq	%rsp, %rbp
LCFI27:
	subq	$16, %rsp
LCFI28:
	movq	%rdi, -8(%rbp)
	leaq	16+__ZTVN4Jpeg6CCJpegE(%rip), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$0, %eax
	testb	%al, %al
	je	L43
	movq	-8(%rbp), %rdi
	call	__ZdlPv
L43:
	leave
	ret
LFE19:
	.align 1
.globl __ZN7ICCJpegD0Ev
	.weak_definition __ZN7ICCJpegD0Ev
__ZN7ICCJpegD0Ev:
LFB10:
	pushq	%rbp
LCFI29:
	movq	%rsp, %rbp
LCFI30:
	subq	$16, %rsp
LCFI31:
	movq	%rdi, -8(%rbp)
	movq	__ZTV7ICCJpeg@GOTPCREL(%rip), %rax
	leaq	16(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$1, %eax
	testb	%al, %al
	je	L48
	movq	-8(%rbp), %rdi
	call	__ZdlPv
L48:
	leave
	ret
LFE10:
	.align 1
.globl __ZN7ICCJpegD1Ev
	.weak_definition __ZN7ICCJpegD1Ev
__ZN7ICCJpegD1Ev:
LFB9:
	pushq	%rbp
LCFI32:
	movq	%rsp, %rbp
LCFI33:
	subq	$16, %rsp
LCFI34:
	movq	%rdi, -8(%rbp)
	movq	__ZTV7ICCJpeg@GOTPCREL(%rip), %rax
	leaq	16(%rax), %rdx
	movq	-8(%rbp), %rax
	movq	%rdx, (%rax)
	movl	$0, %eax
	testb	%al, %al
	je	L53
	movq	-8(%rbp), %rdi
	call	__ZdlPv
L53:
	leave
	ret
LFE9:
	.section __TEXT,__StaticInit,regular,pure_instructions
__Z41__static_initialization_and_destruction_0ii:
LFB1517:
	pushq	%rbp
LCFI35:
	movq	%rsp, %rbp
LCFI36:
	subq	$16, %rsp
LCFI37:
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	L58
	cmpl	$65535, -8(%rbp)
	jne	L58
	leaq	__ZStL8__ioinit(%rip), %rdi
	call	__ZNSt8ios_base4InitC1Ev
	movq	___dso_handle@GOTPCREL(%rip), %rdx
	movl	$0, %esi
	leaq	___tcf_0(%rip), %rdi
	call	___cxa_atexit
L58:
	leave
	ret
LFE1517:
__GLOBAL__I__ZN4Jpeg4destE:
LFB1519:
	pushq	%rbp
LCFI38:
	movq	%rsp, %rbp
LCFI39:
	movl	$65535, %esi
	movl	$1, %edi
	call	__Z41__static_initialization_and_destruction_0ii
	leave
	ret
LFE1519:
	.text
___tcf_0:
LFB1518:
	pushq	%rbp
LCFI40:
	movq	%rsp, %rbp
LCFI41:
	subq	$16, %rsp
LCFI42:
	movq	%rdi, -8(%rbp)
	leaq	__ZStL8__ioinit(%rip), %rdi
	call	__ZNSt8ios_base4InitD1Ev
	leave
	ret
LFE1518:
	.cstring
LC0:
	.ascii "wb\0"
	.align 3
LC1:
	.ascii "/Users/Ashish/test/compress.jpeg\0"
LC2:
	.ascii "jpegLength is \0"
	.text
	.align 1,0x90
.globl __ZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijj
__ZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijj:
LFB1501:
	pushq	%rbp
LCFI43:
	movq	%rsp, %rbp
LCFI44:
	pushq	%rbx
LCFI45:
	subq	$872, %rsp
LCFI46:
	movq	%rdi, -840(%rbp)
	movq	%rsi, -848(%rbp)
	movq	%rdx, -856(%rbp)
	movq	%rcx, -864(%rbp)
	movq	%r8, -872(%rbp)
	movl	%r9d, -876(%rbp)
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rdx
	movq	%rdx, -24(%rbp)
	xorl	%edx, %edx
	movl	__ZZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijjE1i(%rip), %eax
	incl	%eax
	movl	%eax, __ZZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijjE1i(%rip)
	movl	__ZZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijjE1i(%rip), %eax
	cmpl	$9, %eax
	jle	L64
	movl	$0, -880(%rbp)
	jmp	L66
L64:
	leaq	LC0(%rip), %rsi
	leaq	LC1(%rip), %rdi
	call	_fopen
	movq	%rax, -800(%rbp)
	leaq	-192(%rbp), %rdi
	call	_jpeg_std_error
	movq	%rax, -784(%rbp)
	leaq	-784(%rbp), %rdi
	movl	$584, %edx
	movl	$80, %esi
	call	_jpeg_CreateCompress
	movq	-800(%rbp), %rsi
	leaq	-784(%rbp), %rdi
	call	_jpeg_stdio_dest
	movq	_window_width@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, -736(%rbp)
	movq	_window_height@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, -732(%rbp)
	movl	$3, -728(%rbp)
	movl	$2, -724(%rbp)
	leaq	-784(%rbp), %rdi
	call	_jpeg_set_defaults
	leaq	-784(%rbp), %rdi
	movl	$1, %edx
	movl	$75, %esi
	call	_jpeg_set_quality
	leaq	-784(%rbp), %rdi
	movl	$1, %esi
	call	_jpeg_start_compress
	movq	-848(%rbp), %rax
	movq	%rax, -824(%rbp)
	movq	_window_width@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	sall	$2, %eax
	movl	%eax, -788(%rbp)
	jmp	L67
L68:
	movl	-788(%rbp), %eax
	movslq	%eax,%rdi
	call	_malloc
	movq	%rax, -808(%rbp)
	movq	-808(%rbp), %rax
	testq	%rax, %rax
	jne	L69
	movl	$0, -880(%rbp)
	jmp	L66
L69:
	movq	-808(%rbp), %rax
	movq	%rax, -816(%rbp)
	movl	$0, -792(%rbp)
	jmp	L71
L72:
	movq	-824(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-816(%rbp), %rax
	movb	%dl, (%rax)
	incq	-816(%rbp)
	incq	-824(%rbp)
	movq	-824(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-816(%rbp), %rax
	movb	%dl, (%rax)
	incq	-816(%rbp)
	incq	-824(%rbp)
	movq	-824(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-816(%rbp), %rax
	movb	%dl, (%rax)
	incq	-816(%rbp)
	incq	-824(%rbp)
	incq	-824(%rbp)
	incl	-792(%rbp)
L71:
	movq	_window_width@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	cmpl	%eax, -792(%rbp)
	jl	L72
	leaq	-808(%rbp), %rsi
	leaq	-784(%rbp), %rdi
	movl	$1, %edx
	call	_jpeg_write_scanlines
	movq	-808(%rbp), %rdi
	call	_free
L67:
	movl	-444(%rbp), %edx
	movl	-732(%rbp), %eax
	cmpl	%eax, %edx
	jb	L68
	leaq	-784(%rbp), %rdi
	call	_jpeg_finish_compress
	leaq	-784(%rbp), %rdi
	call	__ZN4Jpeg6CCJpeg13jpeg_mem_sizeEP20jpeg_compress_struct
	movl	%eax, %edx
	movq	-872(%rbp), %rax
	movl	%edx, (%rax)
	movq	-872(%rbp), %rax
	movl	(%rax), %eax
	movslq	%eax,%rbx
	leaq	LC2(%rip), %rsi
	movq	__ZSt4cout@GOTPCREL(%rip), %rdi
	call	__ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rax, %rdi
	movq	%rbx, %rsi
	call	__ZNSolsEm
	movq	%rax, %rdi
	movq	__ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rsi
	call	__ZNSolsEPFRSoS_E
	leaq	-784(%rbp), %rdi
	call	_jpeg_destroy_compress
	movq	-800(%rbp), %rdi
	call	_fclose
	movl	$1, -880(%rbp)
L66:
	movl	-880(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rdx
	movq	-24(%rbp), %rcx
	xorq	(%rdx), %rcx
	je	L76
	call	___stack_chk_fail
L76:
	addq	$872, %rsp
	popq	%rbx
	leave
	ret
LFE1501:
	.align 1,0x90
.globl __ZN4Jpeg6CCJpeg13jpeg_compressEPhS1_mPiPjS3_jj
__ZN4Jpeg6CCJpeg13jpeg_compressEPhS1_mPiPjS3_jj:
LFB1500:
	pushq	%rbp
LCFI47:
	movq	%rsp, %rbp
LCFI48:
	subq	$896, %rsp
LCFI49:
	movq	%rdi, -840(%rbp)
	movq	%rsi, -848(%rbp)
	movq	%rdx, -856(%rbp)
	movq	%rcx, -864(%rbp)
	movq	%r8, -872(%rbp)
	movq	%r9, -880(%rbp)
	movq	16(%rbp), %rax
	movq	%rax, -888(%rbp)
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rdx
	movq	%rdx, -8(%rbp)
	xorl	%edx, %edx
	leaq	-176(%rbp), %rdi
	call	_jpeg_std_error
	movq	%rax, -768(%rbp)
	leaq	-768(%rbp), %rdi
	movl	$584, %edx
	movl	$80, %esi
	call	_jpeg_CreateCompress
	movw	$1, -436(%rbp)
	movw	$1, -434(%rbp)
	movq	-864(%rbp), %rdx
	movq	-856(%rbp), %rsi
	leaq	-768(%rbp), %rdi
	call	__ZN4Jpeg6CCJpeg13jpeg_mem_destEP20jpeg_compress_structPhm
	movl	$3, -712(%rbp)
	movl	$2, -708(%rbp)
	leaq	-768(%rbp), %rdi
	call	_jpeg_set_defaults
	leaq	-768(%rbp), %rdi
	movl	$1, %edx
	movl	$75, %esi
	call	_jpeg_set_quality
	movl	24(%rbp), %eax
	movl	%eax, -720(%rbp)
	movl	32(%rbp), %eax
	movl	%eax, -716(%rbp)
	leaq	-768(%rbp), %rdi
	movl	$1, %esi
	call	_jpeg_start_compress
	movl	$0, -776(%rbp)
	jmp	L78
L79:
	movl	-720(%rbp), %eax
	imull	-776(%rbp), %eax
	sall	$2, %eax
	mov	%eax, %eax
	addq	-848(%rbp), %rax
	movq	%rax, -792(%rbp)
	movl	-716(%rbp), %eax
	subl	-776(%rbp), %eax
	sall	$2, %eax
	leal	-4(%rax), %edx
	movl	-720(%rbp), %eax
	imull	%edx, %eax
	mov	%eax, %eax
	addq	-848(%rbp), %rax
	movq	%rax, -800(%rbp)
	movl	$0, -780(%rbp)
	jmp	L80
L81:
	movq	-792(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -769(%rbp)
	movq	-800(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-792(%rbp), %rax
	movb	%dl, (%rax)
	incq	-792(%rbp)
	movq	-800(%rbp), %rdx
	movzbl	-769(%rbp), %eax
	movb	%al, (%rdx)
	incq	-800(%rbp)
	movq	-792(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -769(%rbp)
	movq	-800(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-792(%rbp), %rax
	movb	%dl, (%rax)
	incq	-792(%rbp)
	movq	-800(%rbp), %rdx
	movzbl	-769(%rbp), %eax
	movb	%al, (%rdx)
	incq	-800(%rbp)
	movq	-792(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -769(%rbp)
	movq	-800(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-792(%rbp), %rax
	movb	%dl, (%rax)
	incq	-792(%rbp)
	movq	-800(%rbp), %rdx
	movzbl	-769(%rbp), %eax
	movb	%al, (%rdx)
	incq	-800(%rbp)
	movq	-792(%rbp), %rax
	movzbl	(%rax), %eax
	movb	%al, -769(%rbp)
	movq	-800(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-792(%rbp), %rax
	movb	%dl, (%rax)
	incq	-792(%rbp)
	movq	-800(%rbp), %rdx
	movzbl	-769(%rbp), %eax
	movb	%al, (%rdx)
	incq	-800(%rbp)
	incl	-780(%rbp)
L80:
	movl	-720(%rbp), %eax
	cmpl	-780(%rbp), %eax
	ja	L81
	incl	-776(%rbp)
L78:
	movl	-716(%rbp), %eax
	shrl	%eax
	cmpl	-776(%rbp), %eax
	ja	L79
	movq	-848(%rbp), %rax
	movq	%rax, -808(%rbp)
	movl	-720(%rbp), %eax
	sall	$2, %eax
	mov	%eax, %edi
	call	_malloc
	movq	%rax, -816(%rbp)
	jmp	L84
L85:
	movl	-720(%rbp), %eax
	sall	$2, %eax
	mov	%eax, %edx
	movq	-816(%rbp), %rdi
	movl	$0, %esi
	call	_memset
	movq	-816(%rbp), %rax
	testq	%rax, %rax
	jne	L86
	movl	$0, -892(%rbp)
	jmp	L88
L86:
	movq	-816(%rbp), %rax
	movq	%rax, -824(%rbp)
	movl	$0, -784(%rbp)
	jmp	L89
L90:
	movq	-808(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-824(%rbp), %rax
	movb	%dl, (%rax)
	incq	-824(%rbp)
	incq	-808(%rbp)
	movq	-808(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-824(%rbp), %rax
	movb	%dl, (%rax)
	incq	-824(%rbp)
	incq	-808(%rbp)
	movq	-808(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-824(%rbp), %rax
	movb	%dl, (%rax)
	incq	-824(%rbp)
	incq	-808(%rbp)
	incq	-808(%rbp)
	incl	-784(%rbp)
L89:
	movl	-720(%rbp), %eax
	cmpl	-784(%rbp), %eax
	ja	L90
	leaq	-816(%rbp), %rsi
	leaq	-768(%rbp), %rdi
	movl	$1, %edx
	call	_jpeg_write_scanlines
L84:
	movl	-428(%rbp), %edx
	movl	-716(%rbp), %eax
	cmpl	%eax, %edx
	jb	L85
	movq	-816(%rbp), %rdi
	call	_free
	leaq	-768(%rbp), %rdi
	call	_jpeg_finish_compress
	leaq	-768(%rbp), %rdi
	call	__ZN4Jpeg6CCJpeg13jpeg_mem_sizeEP20jpeg_compress_struct
	movl	%eax, %edx
	movq	-872(%rbp), %rax
	movl	%edx, (%rax)
	leaq	-768(%rbp), %rdi
	call	_jpeg_destroy_compress
	movl	$1, -892(%rbp)
L88:
	movl	-892(%rbp), %eax
	movq	___stack_chk_guard@GOTPCREL(%rip), %rdx
	movq	-8(%rbp), %rcx
	xorq	(%rdx), %rcx
	je	L94
	call	___stack_chk_fail
L94:
	leave
	ret
LFE1500:
	.align 1,0x90
.globl __ZN4Jpeg6CCJpeg21JpegCompressPixelDataEPv
__ZN4Jpeg6CCJpeg21JpegCompressPixelDataEPv:
LFB1496:
	pushq	%rbp
LCFI50:
	movq	%rsp, %rbp
LCFI51:
	pushq	%rbx
LCFI52:
	subq	$72, %rsp
LCFI53:
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	-48(%rbp), %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	movl	28(%rax), %edx
	movq	-24(%rbp), %rax
	movl	24(%rax), %ecx
	movq	-24(%rbp), %r8
	addq	$36, %r8
	movq	-24(%rbp), %r9
	addq	$32, %r9
	movq	-24(%rbp), %r10
	addq	$20, %r10
	movq	-24(%rbp), %rax
	movl	16(%rax), %eax
	mov	%eax, %r11d
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, %rbx
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, %rsi
	movq	-40(%rbp), %rdi
	movl	%edx, 16(%rsp)
	movl	%ecx, 8(%rsp)
	movq	%r8, (%rsp)
	movq	%r10, %r8
	movq	%r11, %rcx
	movq	%rbx, %rdx
	call	__ZN4Jpeg6CCJpeg13jpeg_compressEPhS1_mPiPjS3_jj
	addq	$72, %rsp
	popq	%rbx
	leave
	ret
LFE1496:
.globl __ZN4Jpeg10CreateJpegEv
__ZN4Jpeg10CreateJpegEv:
LFB1495:
	pushq	%rbp
LCFI54:
	movq	%rsp, %rbp
LCFI55:
	pushq	%rbx
LCFI56:
	subq	$8, %rsp
LCFI57:
	movl	$272, %edi
	call	__Znwm
	movq	%rax, %rbx
	movq	%rbx, %rdi
	call	__ZN4Jpeg6CCJpegC1Ev
	movq	%rbx, %rax
	addq	$8, %rsp
	popq	%rbx
	leave
	ret
LFE1495:
.globl __ZN4Jpeg4destE
.zerofill __DATA, __common, __ZN4Jpeg4destE, 8, 3
.globl __ZTVN4Jpeg6CCJpegE
	.const_data
	.align 5
__ZTVN4Jpeg6CCJpegE:
	.quad	0
	.quad	__ZTIN4Jpeg6CCJpegE
	.quad	__ZN4Jpeg6CCJpegD1Ev
	.quad	__ZN4Jpeg6CCJpegD0Ev
	.quad	__ZN4Jpeg6CCJpeg21JpegCompressPixelDataEPv
.globl __ZTSN4Jpeg6CCJpegE
	.cstring
__ZTSN4Jpeg6CCJpegE:
	.ascii "N4Jpeg6CCJpegE\0"
.globl __ZTIN4Jpeg6CCJpegE
	.const_data
	.align 4
__ZTIN4Jpeg6CCJpegE:
	.quad	__ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	__ZTSN4Jpeg6CCJpegE
	.quad	__ZTI7ICCJpeg
.globl __ZTI7ICCJpeg
	.weak_definition __ZTI7ICCJpeg
	.section __DATA,__const_coal,coalesced
	.align 4
__ZTI7ICCJpeg:
	.quad	__ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	__ZTS7ICCJpeg
.globl __ZTS7ICCJpeg
	.weak_definition __ZTS7ICCJpeg
	.section __TEXT,__const_coal,coalesced
__ZTS7ICCJpeg:
	.ascii "7ICCJpeg\0"
.lcomm __ZStL8__ioinit,1,0
.lcomm __ZZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijjE1i,4,2
.globl __ZTV7ICCJpeg
	.weak_definition __ZTV7ICCJpeg
	.section __DATA,__const_coal,coalesced
	.align 5
__ZTV7ICCJpeg:
	.quad	0
	.quad	__ZTI7ICCJpeg
	.quad	__ZN7ICCJpegD1Ev
	.quad	__ZN7ICCJpegD0Ev
	.quad	___cxa_pure_virtual
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0x0
	.byte	0x1
	.ascii "zPR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x6
	.byte	0x9b
	.long	___gxx_personality_v0+4@GOTPCREL
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
.globl __ZN7ICCJpegC2Ev.eh
	.weak_definition __ZN7ICCJpegC2Ev.eh
__ZN7ICCJpegC2Ev.eh:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB13-.
	.set L$set$2,LFE13-LFB13
	.quad L$set$2
	.byte	0x0
	.byte	0x4
	.set L$set$3,LCFI0-LFB13
	.long L$set$3
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE1:
.globl __ZN4Jpeg6CCJpegC1Ev.eh
	.weak_definition __ZN4Jpeg6CCJpegC1Ev.eh
__ZN4Jpeg6CCJpegC1Ev.eh:
LSFDE3:
	.set L$set$5,LEFDE3-LASFDE3
	.long L$set$5
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB16-.
	.set L$set$6,LFE16-LFB16
	.quad L$set$6
	.byte	0x0
	.byte	0x4
	.set L$set$7,LCFI2-LFB16
	.long L$set$7
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$8,LCFI3-LCFI2
	.long L$set$8
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE3:
.globl __ZN4Jpeg16init_destinationEP20jpeg_compress_struct.eh
__ZN4Jpeg16init_destinationEP20jpeg_compress_struct.eh:
LSFDE5:
	.set L$set$9,LEFDE5-LASFDE5
	.long L$set$9
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB1497-.
	.set L$set$10,LFE1497-LFB1497
	.quad L$set$10
	.byte	0x0
	.byte	0x4
	.set L$set$11,LCFI5-LFB1497
	.long L$set$11
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$12,LCFI6-LCFI5
	.long L$set$12
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE5:
.globl __ZN4Jpeg19empty_output_bufferEP20jpeg_compress_struct.eh
__ZN4Jpeg19empty_output_bufferEP20jpeg_compress_struct.eh:
LSFDE7:
	.set L$set$13,LEFDE7-LASFDE7
	.long L$set$13
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB1498-.
	.set L$set$14,LFE1498-LFB1498
	.quad L$set$14
	.byte	0x0
	.byte	0x4
	.set L$set$15,LCFI7-LFB1498
	.long L$set$15
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$16,LCFI8-LCFI7
	.long L$set$16
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE7:
.globl __ZN4Jpeg16term_destinationEP20jpeg_compress_struct.eh
__ZN4Jpeg16term_destinationEP20jpeg_compress_struct.eh:
LSFDE9:
	.set L$set$17,LEFDE9-LASFDE9
	.long L$set$17
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB1499-.
	.set L$set$18,LFE1499-LFB1499
	.quad L$set$18
	.byte	0x0
	.byte	0x4
	.set L$set$19,LCFI9-LFB1499
	.long L$set$19
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$20,LCFI10-LCFI9
	.long L$set$20
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$21,LCFI11-LCFI10
	.long L$set$21
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE9:
.globl __ZN4Jpeg6CCJpeg13jpeg_mem_destEP20jpeg_compress_structPhm.eh
__ZN4Jpeg6CCJpeg13jpeg_mem_destEP20jpeg_compress_structPhm.eh:
LSFDE11:
	.set L$set$22,LEFDE11-LASFDE11
	.long L$set$22
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB1502-.
	.set L$set$23,LFE1502-LFB1502
	.quad L$set$23
	.byte	0x0
	.byte	0x4
	.set L$set$24,LCFI12-LFB1502
	.long L$set$24
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$25,LCFI13-LCFI12
	.long L$set$25
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE11:
.globl __ZN4Jpeg6CCJpeg13jpeg_mem_sizeEP20jpeg_compress_struct.eh
__ZN4Jpeg6CCJpeg13jpeg_mem_sizeEP20jpeg_compress_struct.eh:
LSFDE13:
	.set L$set$26,LEFDE13-LASFDE13
	.long L$set$26
LASFDE13:
	.long	LASFDE13-EH_frame1
	.quad	LFB1503-.
	.set L$set$27,LFE1503-LFB1503
	.quad L$set$27
	.byte	0x0
	.byte	0x4
	.set L$set$28,LCFI15-LFB1503
	.long L$set$28
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$29,LCFI16-LCFI15
	.long L$set$29
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE13:
.globl __ZSt3minImERKT_S2_S2_.eh
	.weak_definition __ZSt3minImERKT_S2_S2_.eh
__ZSt3minImERKT_S2_S2_.eh:
LSFDE15:
	.set L$set$30,LEFDE15-LASFDE15
	.long L$set$30
LASFDE15:
	.long	LASFDE15-EH_frame1
	.quad	LFB1504-.
	.set L$set$31,LFE1504-LFB1504
	.quad L$set$31
	.byte	0x0
	.byte	0x4
	.set L$set$32,LCFI17-LFB1504
	.long L$set$32
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$33,LCFI18-LCFI17
	.long L$set$33
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE15:
__ZStL17__verify_groupingPKcmRKSs.eh:
LSFDE17:
	.set L$set$34,LEFDE17-LASFDE17
	.long L$set$34
LASFDE17:
	.long	LASFDE17-EH_frame1
	.quad	LFB1425-.
	.set L$set$35,LFE1425-LFB1425
	.quad L$set$35
	.byte	0x0
	.byte	0x4
	.set L$set$36,LCFI19-LFB1425
	.long L$set$36
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$37,LCFI20-LCFI19
	.long L$set$37
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$38,LCFI22-LCFI20
	.long L$set$38
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE17:
.globl __ZN4Jpeg6CCJpegD0Ev.eh
	.weak_definition __ZN4Jpeg6CCJpegD0Ev.eh
__ZN4Jpeg6CCJpegD0Ev.eh:
LSFDE19:
	.set L$set$39,LEFDE19-LASFDE19
	.long L$set$39
LASFDE19:
	.long	LASFDE19-EH_frame1
	.quad	LFB20-.
	.set L$set$40,LFE20-LFB20
	.quad L$set$40
	.byte	0x0
	.byte	0x4
	.set L$set$41,LCFI23-LFB20
	.long L$set$41
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$42,LCFI24-LCFI23
	.long L$set$42
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE19:
.globl __ZN4Jpeg6CCJpegD1Ev.eh
	.weak_definition __ZN4Jpeg6CCJpegD1Ev.eh
__ZN4Jpeg6CCJpegD1Ev.eh:
LSFDE21:
	.set L$set$43,LEFDE21-LASFDE21
	.long L$set$43
LASFDE21:
	.long	LASFDE21-EH_frame1
	.quad	LFB19-.
	.set L$set$44,LFE19-LFB19
	.quad L$set$44
	.byte	0x0
	.byte	0x4
	.set L$set$45,LCFI26-LFB19
	.long L$set$45
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$46,LCFI27-LCFI26
	.long L$set$46
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE21:
.globl __ZN7ICCJpegD0Ev.eh
	.weak_definition __ZN7ICCJpegD0Ev.eh
__ZN7ICCJpegD0Ev.eh:
LSFDE23:
	.set L$set$47,LEFDE23-LASFDE23
	.long L$set$47
LASFDE23:
	.long	LASFDE23-EH_frame1
	.quad	LFB10-.
	.set L$set$48,LFE10-LFB10
	.quad L$set$48
	.byte	0x0
	.byte	0x4
	.set L$set$49,LCFI29-LFB10
	.long L$set$49
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$50,LCFI30-LCFI29
	.long L$set$50
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE23:
.globl __ZN7ICCJpegD1Ev.eh
	.weak_definition __ZN7ICCJpegD1Ev.eh
__ZN7ICCJpegD1Ev.eh:
LSFDE25:
	.set L$set$51,LEFDE25-LASFDE25
	.long L$set$51
LASFDE25:
	.long	LASFDE25-EH_frame1
	.quad	LFB9-.
	.set L$set$52,LFE9-LFB9
	.quad L$set$52
	.byte	0x0
	.byte	0x4
	.set L$set$53,LCFI32-LFB9
	.long L$set$53
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$54,LCFI33-LCFI32
	.long L$set$54
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE25:
__Z41__static_initialization_and_destruction_0ii.eh:
LSFDE27:
	.set L$set$55,LEFDE27-LASFDE27
	.long L$set$55
LASFDE27:
	.long	LASFDE27-EH_frame1
	.quad	LFB1517-.
	.set L$set$56,LFE1517-LFB1517
	.quad L$set$56
	.byte	0x0
	.byte	0x4
	.set L$set$57,LCFI35-LFB1517
	.long L$set$57
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$58,LCFI36-LCFI35
	.long L$set$58
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE27:
__GLOBAL__I__ZN4Jpeg4destE.eh:
LSFDE29:
	.set L$set$59,LEFDE29-LASFDE29
	.long L$set$59
LASFDE29:
	.long	LASFDE29-EH_frame1
	.quad	LFB1519-.
	.set L$set$60,LFE1519-LFB1519
	.quad L$set$60
	.byte	0x0
	.byte	0x4
	.set L$set$61,LCFI38-LFB1519
	.long L$set$61
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$62,LCFI39-LCFI38
	.long L$set$62
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE29:
___tcf_0.eh:
LSFDE31:
	.set L$set$63,LEFDE31-LASFDE31
	.long L$set$63
LASFDE31:
	.long	LASFDE31-EH_frame1
	.quad	LFB1518-.
	.set L$set$64,LFE1518-LFB1518
	.quad L$set$64
	.byte	0x0
	.byte	0x4
	.set L$set$65,LCFI40-LFB1518
	.long L$set$65
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$66,LCFI41-LCFI40
	.long L$set$66
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE31:
.globl __ZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijj.eh
__ZN4Jpeg6CCJpeg21jpeg_compress_in_fileEPhS1_mPijj.eh:
LSFDE33:
	.set L$set$67,LEFDE33-LASFDE33
	.long L$set$67
LASFDE33:
	.long	LASFDE33-EH_frame1
	.quad	LFB1501-.
	.set L$set$68,LFE1501-LFB1501
	.quad L$set$68
	.byte	0x0
	.byte	0x4
	.set L$set$69,LCFI43-LFB1501
	.long L$set$69
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$70,LCFI44-LCFI43
	.long L$set$70
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$71,LCFI46-LCFI44
	.long L$set$71
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE33:
.globl __ZN4Jpeg6CCJpeg13jpeg_compressEPhS1_mPiPjS3_jj.eh
__ZN4Jpeg6CCJpeg13jpeg_compressEPhS1_mPiPjS3_jj.eh:
LSFDE35:
	.set L$set$72,LEFDE35-LASFDE35
	.long L$set$72
LASFDE35:
	.long	LASFDE35-EH_frame1
	.quad	LFB1500-.
	.set L$set$73,LFE1500-LFB1500
	.quad L$set$73
	.byte	0x0
	.byte	0x4
	.set L$set$74,LCFI47-LFB1500
	.long L$set$74
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$75,LCFI48-LCFI47
	.long L$set$75
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE35:
.globl __ZN4Jpeg6CCJpeg21JpegCompressPixelDataEPv.eh
__ZN4Jpeg6CCJpeg21JpegCompressPixelDataEPv.eh:
LSFDE37:
	.set L$set$76,LEFDE37-LASFDE37
	.long L$set$76
LASFDE37:
	.long	LASFDE37-EH_frame1
	.quad	LFB1496-.
	.set L$set$77,LFE1496-LFB1496
	.quad L$set$77
	.byte	0x0
	.byte	0x4
	.set L$set$78,LCFI50-LFB1496
	.long L$set$78
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$79,LCFI51-LCFI50
	.long L$set$79
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$80,LCFI53-LCFI51
	.long L$set$80
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE37:
.globl __ZN4Jpeg10CreateJpegEv.eh
__ZN4Jpeg10CreateJpegEv.eh:
LSFDE39:
	.set L$set$81,LEFDE39-LASFDE39
	.long L$set$81
LASFDE39:
	.long	LASFDE39-EH_frame1
	.quad	LFB1495-.
	.set L$set$82,LFE1495-LFB1495
	.quad L$set$82
	.byte	0x0
	.byte	0x4
	.set L$set$83,LCFI54-LFB1495
	.long L$set$83
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$84,LCFI55-LCFI54
	.long L$set$84
	.byte	0xd
	.byte	0x6
	.byte	0x4
	.set L$set$85,LCFI57-LCFI55
	.long L$set$85
	.byte	0x83
	.byte	0x3
	.align 3
LEFDE39:
	.constructor
	.destructor
	.align 1
	.subsections_via_symbols
