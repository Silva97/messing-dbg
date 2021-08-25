#include <stdio.h>

#define MESS_CALL(func, ...) \
  func(__VA_ARGS__);         \
  asm("nop")

#define MESS_RET(...)    \
  asm("add $1, (%rsp)"); \
  return __VA_ARGS__

void test(void)
{
  int a = 5;
  MESS_RET();
}

int main(void)
{
  MESS_CALL(test);
  puts("Hello World!");
}
