#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <ucontext.h>

#define SOL 99
#define ULT_MIN_STACK 32768 

void foo();

int flag = 0;

ucontext_t my_context;

int main(int argc, char **argv)
{

  getcontext(&my_context);

  unsigned int *new_stack = malloc(ULT_MIN_STACK);

  my_context.uc_stack.ss_size = ULT_MIN_STACK;
  my_context.uc_stack.ss_sp = new_stack;

  new_stack += (ULT_MIN_STACK/4);
  (new_stack)--;
  *new_stack = (unsigned int) argv;
  (new_stack)--;
  *new_stack = (unsigned int) argc;

  my_context.uc_mcontext.gregs[REG_EIP] = (unsigned int)foo;
  my_context.uc_mcontext.gregs[REG_ESP] = (unsigned int)new_stack;

  foo();

  return 0;

}

void foo()
{
  flag++;
  printf("Hello World\n");
  my_context.uc_mcontext.gregs[REG_EIP] = (unsigned int)main;
  setcontext(&my_context); 
}
