#include <stdio.h>
#include "hello.h"

void extfunc1(int x);
void extfunc3(double param1, long double param2);
void extfunc4(char* stringa);

int extfunc2(double x, int y);

/*int main(int argc, char **argv)
{
  function(4,5);
  }*/

int function(int x, int y)
{
  int w = 2 + extfunc2(5.4, 27);
  extfunc1(51);
  extfunc4("Hello!");
  return x;
  // return extfunc_1(4); Check!
}




