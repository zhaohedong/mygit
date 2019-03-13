# A program to be called from a C program
# Declaring data that doesn't change
.section .data
    string: .ascii  "Hello from assembler\n"
    length: .quad   . - string
    msg: .ascii "Hello World\n"
    .comm vendor_id,20
# The actual code
.section .text
.global print
.global print2
.global myprint
.type print, @function              #<-Important
.type print2, @function              #<-Important
.type myprint, @function              #<-Important

print2:
    mov $4, %rax   # use the write syscall
    mov $1, %rbx   # write to stdout
    mov $msg, %rcx # use string "Hello World"
    mov $12, %rdx  # write 12 characters
    int $0x80      # make syscall
    
    mov $1, %rax   # use the _exit syscall
    mov $0, %rbx   # error code 0
    int $0x80      # make syscall

myprint:
    xor %rax, %rax
    cpuid
    mov $vendor_id, %rdi
    mov %ebx, 0(%rdi)
    mov %edx, 4(%rdi)
    mov %ecx, 8(%rdi)
    
    mov $4, %rax   # use the write syscall
    mov $1, %rbx   # write to stdout
    mov $vendor_id,%rcx
    mov $20, %rdx  # write 24 characters
    int $0x80      # make syscall
    
    mov $1, %rax   # use the _exit syscall
    mov $0, %rbx   # error code 0
    int $0x80      # make syscall

print:
    xor %rax, %rax
    cpuid
#    rdrand
    mov     $0x1,%rax               # Move 1(write) into rax
    mov     $0x1,%rdi               # Move 1(fd stdOut) into rdi.
    mov     $string,%rsi            # Move the _location_ of the string into rsi
    mov     length,%rdx             # Move the _length_ of the string into rdx
    syscall                         # Call the kernel

    mov     %rax,%rdi               # Move the number of bytes written to rdi
    mov     $0x3c,%rax              # Move 60(sys_exit) into rax
    syscall                         # Call the kernel
