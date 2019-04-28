# A program to be called from a C program
# Declaring data that doesn't change
.section .data
    .comm vendor_id,20
    carriageReturn: .ascii  "This is a cr\n"
# The actual code
.section .text
.global myprint
.global print3
.global print4
.type myprint, @function              #<-Important
.type print3, @function              #<-Important
.type print4, @function              #<-Important

myprint:
#get cpuid info
    xor %rax, %rax
    cpuid
    mov $vendor_id, %rdi # mov the address of vendor_id to rdi
    mov %ebx, (%rdi)     # mov the value in ebx to the address point by rdi
    mov %edx, 4(%rdi)    # mov the value in edx to the address point by rdi+4
    mov %ecx, 8(%rdi)    # mov the value in edx to the address point by rdi+8

#output cpuid info    
    mov $4, %rax   # use the write syscall
    mov $1, %rbx   # write to stdout
    mov $vendor_id,%rcx
    mov $20, %rdx  # write 20 characters
    int $0x80      # make syscall

#carriage return
#    mov $4, %rax   # use the write syscall
#    mov $1, %rbx   # write to stdout
#    mov $carriageReturn,%rcx
#    mov $20, %rdx  # write 20 characters
#    int $0x80      # make syscall

#exit
#    mov $1, %rax   # use the _exit syscall
#    mov $0, %rbx   # error code 0
#    int $0x80      # make syscall

print3:
        pushq   %rbp
        movq    %rsp, %rbp
        movl    %edi, -20(%rbp)
        movl    %esi, -24(%rbp)
        movl    %edx, -28(%rbp)
        movl    -20(%rbp), %edx
        movl    -24(%rbp), %eax
        addl    %eax, %edx
        movl    -28(%rbp), %eax
        addl    %edx, %eax
        movl    %eax, -4(%rbp)
        nop
        popq    %rbp
        ret

print4:
        pushq   %rbp
        movq    %rsp, %rbp
        movq    %rdi, -8(%rbp)
        movl    %esi, -12(%rbp)
        movl    %edx, -16(%rbp)
        movl    %ecx, -20(%rbp)
#output cpuid info    
        mov $4, %rax   # use the write syscall
        mov $1, %rbx   # write to stdout
        mov -8(%rbp),%rcx
        #mov $carriageReturn,%rcx
        mov $20, %rdx  # write 20 characters
        int $0x80      # make syscall
        movl    $0, %eax
        popq    %rbp
        ret
