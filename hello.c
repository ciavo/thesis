#include <stdio.h>

double foo(double x);
int  func(double x, int y);

void bar(char*  stringa);
//extern int xxxxxx;
int function(int x, int y)
{

  double z = foo(7.4);
  printf("%g\n",z); 
  
  z = foo(7.4); 
  printf("%g\n",z);

  z = foo(7.4); 
 printf("%g\n",z);

  z = foo(7.4); 
 printf("%g\n",z);

  z = foo(7.4); 
  printf("%g\n",z); //foo is called 5 times

  // z = foo(7.4);

  int w =  func(5.4, 27);
  printf("from func: %d\n",w);

  bar("Hello!");
  //bar("another Hello!");
  // bar("Hello!");

  return 1;
  // return extfunc_1(4); Check!
}




