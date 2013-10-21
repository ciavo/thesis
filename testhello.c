#include <stdio.h>
#include "myframework.h"

#include "hello.h"
//#include "temporary_file.h"

// return 0 if the test is positive. Otherwise 0
int myfunction_for_test(int x){
  printf("myfunction_for_test!!! x = %d \n", x);
  if(x>56){
    return 0;
  }else return 1;
}

void testhello(){

  expect_extfunc2(VALUE(5.4),VALUE(27));
  expect_extfunc1(USER_FUNCTION(&myfunction_for_test));
  expect_extfunc4(STRING("Hello!"));
  function(2,3);
}
int main(int argc, char **argv)
{
  testhello();
}

