#include <stdio.h>

int foo(int x);

int function_to_test()
{
  int w =  foo(6);
  w = foo(6);

  printf("print w: %d \n",w);
  return w;
}

