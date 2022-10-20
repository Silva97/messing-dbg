#include <stdio.h>
#define __USE_GNU
#include <signal.h>

#define BPCALL_DECLARE(ret_type)                                                    \
  __attribute__((noinline, naked)) static ret_type bpcall_##ret_type(int proc, ...) \
  {                                                                                 \
    asm("int3\n\t"                                                                  \
        "ret");                                                                     \
  }

enum
{
  PROC_SUM,
};

BPCALL_DECLARE(int)

int sum(int a, int b)
{
  return a + b;
}

void breakpoint_handler(int signum, siginfo_t *info, void *context)
{
  static void *procedures[] = {
      [PROC_SUM] = sum,
  };

  mcontext_t *mcontext = &((ucontext_t *)context)->uc_mcontext;

  asm("mov %[val_rax], %%rax\n\t"
      "mov %[val_rsi], %%rdi\n\t"
      "mov %[val_rdx], %%rsi\n\t"
      "mov %[val_rcx], %%rdx\n\t"
      "mov %[val_r8], %%rcx\n\t"
      "mov %[val_r9], %%r8\n\t"
      "call *%[proc_addr]\n\t"
      "mov %%rax, %[ret_value]\n\t"

      : [ret_value] "=m"(mcontext->gregs[REG_RAX])

      : [proc_addr] "m"(procedures[mcontext->gregs[REG_RDI]]),
        [val_rax] "m"(mcontext->gregs[REG_RAX]),
        [val_rsi] "m"(mcontext->gregs[REG_RSI]),
        [val_rdx] "m"(mcontext->gregs[REG_RDX]),
        [val_rcx] "m"(mcontext->gregs[REG_RCX]),
        [val_r8] "m"(mcontext->gregs[REG_R8]),
        [val_r9] "m"(mcontext->gregs[REG_R9])
      : "rax",
        "rdi",
        "rsi",
        "rdx",
        "rcx",
        "r8");
}

int main(void)
{
  struct sigaction breakpoint_action = {
      .sa_flags = SA_SIGINFO,
      .sa_sigaction = breakpoint_handler,
  };

  sigaction(SIGTRAP, &breakpoint_action, NULL);

  printf("%d\n", bpcall_int(PROC_SUM, 5, bpcall_int(PROC_SUM, 2, 3)));
}
