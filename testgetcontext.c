#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <ucontext.h>

#define SOL 99

void foo();

int main(int argc, char **argv)
{

  ucontext_t mycontext;
  getcontext(&mycontext);
  mycontext.uc_mcontext.gregs[REG_EIP] = (unsigned int)&foo;
  setcontext(&mycontext);

  return 0;

}

void foo()
{
  printf("Hello World\n");
}
