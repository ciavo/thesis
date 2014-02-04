#include <stdio.h>
#include "myframework.h"
//#include "temporary_user_structs.h"
//#include "system_under_test.h"
#include "temporary_file.h" //import MACROs for the expectation functions


int myfunction( void (*timerCallback_func)(void)){
  return 0;//positive 
}


void test_sut(){
 
  /* EXPECT_set_sex(ANY_VALUE, 
	         USER_FUNCTION(&check_sex_male),
	         TIMES(1)
	    );
  */
  EXPECT_LPP_internalSetHwTimerCallback( 
	         USER_FUNCTION(&myfunction),
	         TIMES(1)
	    );
  

  function_to_test(); //Execution: Call the function to test!
}
int main(int argc, char **argv)
{
  test_sut();
  TEST_FINISHED;
  return 1;
}

