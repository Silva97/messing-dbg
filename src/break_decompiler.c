#include <stdio.h>

#define BREAK_DECOMPILER()                \
  asm(                                    \
      "lea .retaddr_%=(%%rip), %%rax\n\t" \
      "mov -8(%%rsp), %%rbx\n\t"          \
      "push %%rax\n\t"                    \
      "ret\n\t"                           \
      ".retaddr_%=:\n\t"                  \
      "mov %%rbx, -8(%%rsp)"              \
      :                                   \
      :                                   \
      : "rax", "rbx")

int sum(int a, int b)
{
  BREAK_DECOMPILER();

  return a + b;
}

int main(void)
{
  BREAK_DECOMPILER();

  printf("%d\n", sum(2, 3));
  return 0;
}
