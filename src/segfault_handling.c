#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define __USE_GNU
#include <signal.h>

#define MAGIC_RAX 0xf000000fabcd1234
#define SEGFAULT_ME()                     \
  asm volatile("movq %[magic], %%rax\n\t" \
               "movb $1, (%%rax)"         \
               :                          \
               : [magic] "i"(MAGIC_RAX)   \
               : "rax")

static _Thread_local int valid_memory;

void ill_be_back(int signum, siginfo_t *info, void *context)
{
  mcontext_t *mcontext = &((ucontext_t *)context)->uc_mcontext;

  if (mcontext->gregs[REG_RAX] != MAGIC_RAX)
  {
    puts("Segmentation fault (core dumped)");
    exit(139);
  }

  mcontext->gregs[REG_RAX] = (greg_t)&valid_memory;
}

int main(void)
{
  float test;
  struct sigaction action = {
      .sa_flags = SA_SIGINFO,
      .sa_sigaction = ill_be_back,
  };

  sigaction(SIGSEGV, &action, NULL);
  SEGFAULT_ME();

  puts("Hello World!");
}
