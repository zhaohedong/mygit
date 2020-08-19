	.file	"cpuid.cpp"
	.section	.text._ZN5CPUIDC2Ej,"axG",@progbits,_ZN5CPUIDC5Ej,comdat
	.align 2
	.weak	_ZN5CPUIDC2Ej
	.type	_ZN5CPUIDC2Ej, @function
_ZN5CPUIDC2Ej:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	.cfi_offset 3, -24
	movq	%rdi, -16(%rbp)
	movl	%esi, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	$0, %edx
	movl	%edx, %ecx
#APP
# 13 "CPUID.h" 1
	cpuid
# 0 "" 2
#NO_APP
	movl	%ebx, %esi
	movl	%eax, %edi
	movq	-16(%rbp), %rax
	movl	%edi, (%rax)
	movq	-16(%rbp), %rax
	movl	%esi, 4(%rax)
	movq	-16(%rbp), %rax
	movl	%ecx, 8(%rax)
	movq	-16(%rbp), %rax
	movl	%edx, 12(%rax)
	nop
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_ZN5CPUIDC2Ej, .-_ZN5CPUIDC2Ej
	.weak	_ZN5CPUIDC1Ej
	.set	_ZN5CPUIDC1Ej,_ZN5CPUIDC2Ej
	.section	.text._ZNK5CPUID3EBXEv,"axG",@progbits,_ZNK5CPUID3EBXEv,comdat
	.align 2
	.weak	_ZNK5CPUID3EBXEv
	.type	_ZNK5CPUID3EBXEv, @function
_ZNK5CPUID3EBXEv:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	addq	$4, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	_ZNK5CPUID3EBXEv, .-_ZNK5CPUID3EBXEv
	.section	.text._ZNK5CPUID3ECXEv,"axG",@progbits,_ZNK5CPUID3ECXEv,comdat
	.align 2
	.weak	_ZNK5CPUID3ECXEv
	.type	_ZNK5CPUID3ECXEv, @function
_ZNK5CPUID3ECXEv:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	addq	$8, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	_ZNK5CPUID3ECXEv, .-_ZNK5CPUID3ECXEv
	.section	.text._ZNK5CPUID3EDXEv,"axG",@progbits,_ZNK5CPUID3EDXEv,comdat
	.align 2
	.weak	_ZNK5CPUID3EDXEv
	.type	_ZNK5CPUID3EDXEv, @function
_ZNK5CPUID3EDXEv:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	addq	$12, %rax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	_ZNK5CPUID3EDXEv, .-_ZNK5CPUID3EDXEv
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata
.LC0:
	.string	"CPU vendor = "
	.text
	.globl	main
	.type	main, @function
main:
.LFB1028:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA1028
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$200, %rsp
	.cfi_offset 3, -24
	movl	%edi, -196(%rbp)
	movq	%rsi, -208(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-176(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	_ZN5CPUIDC1Ej
	leaq	-160(%rbp), %rax
	movq	%rax, %rdi
.LEHB0:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1Ev
.LEHE0:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcEC1Ev
	leaq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNK5CPUID3EBXEv
	movq	%rax, %rsi
	leaq	-177(%rbp), %rdx
	leaq	-128(%rbp), %rax
	movq	%rdx, %rcx
	movl	$4, %edx
	movq	%rax, %rdi
.LEHB1:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcmRKS3_
.LEHE1:
	leaq	-128(%rbp), %rdx
	leaq	-160(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
.LEHB2:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEpLERKS4_
.LEHE2:
	leaq	-128(%rbp), %rax
	movq	%rax, %rdi
.LEHB3:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
.LEHE3:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcEC1Ev
	leaq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNK5CPUID3EDXEv
	movq	%rax, %rsi
	leaq	-177(%rbp), %rdx
	leaq	-96(%rbp), %rax
	movq	%rdx, %rcx
	movl	$4, %edx
	movq	%rax, %rdi
.LEHB4:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcmRKS3_
.LEHE4:
	leaq	-96(%rbp), %rdx
	leaq	-160(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
.LEHB5:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEpLERKS4_
.LEHE5:
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
.LEHB6:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
.LEHE6:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcEC1Ev
	leaq	-176(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNK5CPUID3ECXEv
	movq	%rax, %rsi
	leaq	-177(%rbp), %rdx
	leaq	-64(%rbp), %rax
	movq	%rdx, %rcx
	movl	$4, %edx
	movq	%rax, %rdi
.LEHB7:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEC1EPKcmRKS3_
.LEHE7:
	leaq	-64(%rbp), %rdx
	leaq	-160(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
.LEHB8:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEpLERKS4_
.LEHE8:
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
.LEHB9:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
.LEHE9:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev
	movl	$.LC0, %esi
	movl	$_ZSt4cout, %edi
.LEHB10:
	call	_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc
	movq	%rax, %rdx
	leaq	-160(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	_ZStlsIcSt11char_traitsIcESaIcEERSt13basic_ostreamIT_T0_ES7_RKNSt7__cxx1112basic_stringIS4_S5_T1_EE
	movl	$_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi
	movq	%rax, %rdi
	call	_ZNSolsEPFRSoS_E
.LEHE10:
	movl	$0, %ebx
	leaq	-160(%rbp), %rax
	movq	%rax, %rdi
.LEHB11:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
.LEHE11:
	movl	%ebx, %eax
	movq	-24(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L17
	jmp	.L25
.L19:
	movq	%rax, %rbx
	leaq	-128(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
	jmp	.L11
.L18:
	movq	%rax, %rbx
.L11:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev
	jmp	.L12
.L22:
	movq	%rax, %rbx
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
	jmp	.L14
.L21:
	movq	%rax, %rbx
.L14:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev
	jmp	.L12
.L24:
	movq	%rax, %rbx
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
	jmp	.L16
.L23:
	movq	%rax, %rbx
.L16:
	leaq	-177(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSaIcED1Ev
	jmp	.L12
.L20:
	movq	%rax, %rbx
.L12:
	leaq	-160(%rbp), %rax
	movq	%rax, %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEED1Ev
	movq	%rbx, %rax
	movq	%rax, %rdi
.LEHB12:
	call	_Unwind_Resume
.LEHE12:
.L25:
	call	__stack_chk_fail
.L17:
	addq	$200, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1028:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1028:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1028-.LLSDACSB1028
.LLSDACSB1028:
	.uleb128 .LEHB0-.LFB1028
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB1028
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L18-.LFB1028
	.uleb128 0
	.uleb128 .LEHB2-.LFB1028
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L19-.LFB1028
	.uleb128 0
	.uleb128 .LEHB3-.LFB1028
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L18-.LFB1028
	.uleb128 0
	.uleb128 .LEHB4-.LFB1028
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L21-.LFB1028
	.uleb128 0
	.uleb128 .LEHB5-.LFB1028
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L22-.LFB1028
	.uleb128 0
	.uleb128 .LEHB6-.LFB1028
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L21-.LFB1028
	.uleb128 0
	.uleb128 .LEHB7-.LFB1028
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L23-.LFB1028
	.uleb128 0
	.uleb128 .LEHB8-.LFB1028
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L24-.LFB1028
	.uleb128 0
	.uleb128 .LEHB9-.LFB1028
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L23-.LFB1028
	.uleb128 0
	.uleb128 .LEHB10-.LFB1028
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L20-.LFB1028
	.uleb128 0
	.uleb128 .LEHB11-.LFB1028
	.uleb128 .LEHE11-.LEHB11
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB12-.LFB1028
	.uleb128 .LEHE12-.LEHB12
	.uleb128 0
	.uleb128 0
.LLSDACSE1028:
	.text
	.size	main, .-main
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1097:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L28
	cmpl	$65535, -8(%rbp)
	jne	.L28
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	call	__cxa_atexit
.L28:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1097:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB1098:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1098:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.11) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
