#include <stdio.h>
//#include "system_under_test.h"

//void set_sex(struct person, enum SEX);
void LPP_internalSetHwTimerCallback( void (*timerCallback_func)(void));
void foo(double x);

void func();
int function_to_test()
{
  LPP_internalSetHwTimerCallback(func);
    
  return 1;
}
