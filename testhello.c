#include <stdio.h>
#include "myframework.h"
//#include "hello.h" //to import the function to test
#include "temporary_file.h"


// return 0 if the test is POSITIVE. 
int myfunction_for_test(double x){
  printf("myfunction_for_test!!! \n");
  if( (x > 7.3f) &&  (x < 7.5f) ){
    return 0;
  }else return 1;
}

void testhello(){
 
  EXPECT_func( Gt(5.3), Ne(28),
	      TIMES(1),
	      WILLONCE(RETURN(4))
	     );
  
  EXPECT_foo(USER_FUNCTION(&myfunction_for_test),
	    TIMES(6),
	    WILLONCE( RETURN(5.6) ),
	    WILL_REPEATEDLY( RETURN(4.8) )
	    );

  
  EXPECT_bar(STRING("Hello!"), TIMES(1));

  function(2,3); //Execution: Call the function to test!
  TEST_FINISHED;
}
int main(int argc, char **argv)
{
  testhello();
  return 1;
}

