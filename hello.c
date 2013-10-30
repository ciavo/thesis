#include <stdio.h>
//#include "hello.h"
double foo(double x);
void bar(char*  stringa);
int  func(double x, int y);

int function(int x, int y)
{
  double z = foo(7.4);
  printf("%g\n",foo(7.4)); 

 /*
  printf(" %g \n", foo(7.4));
  printf(" %g \n", foo(7.4));
  printf(" %g \n", foo(7.4));
  printf(" %g \n", foo(7.4));
 
  */
  z = foo(7.4); 
 printf("%g\n",z);

  z = foo(7.4); 
 printf("%g\n",z);
  z = foo(7.4); 
 printf("%g\n",z);
  z = foo(7.4); 
 printf("%g\n",z); 

  int w =  func(5.4, 27);
  printf("%d\n",w);
  //bar("Hello!");  
  return 1;
  // return extfunc_1(4); Check!
}




