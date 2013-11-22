#include <stdio.h>

int foo(int x);


int bar(){
  return 1;

}
int baz();

int function_to_test()
{
  baz();  
  //int w =  foo(6);
int w;  
int x = bar();
 printf("hello world!");
  printf("print w: %d \n",w);
  return w;
}

