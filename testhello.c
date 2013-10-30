#include <stdio.h>
#include "myframework.h"
//#include "hello.h" //to import the function to test
#include "temporary_file.h"

// return 0 if the test is positive. Otherwise 0
int myfunction_for_test(double x){
  printf("myfunction_for_test!!! \n");
  if(x < 7.5f ){
    return 0;
  }else return 1;
}

void testhello(){
 
 EXPECT_func( VALUE(5.4), VALUE(27),
	      TIMES(2),
	      WILLONCE(RETURN(4)),
	      WILLONCE(RETURN(5))
	     );
  
 EXPECT_foo(USER_FUNCTION(&myfunction_for_test),
	    TIMES(4),
	    WILLONCE( RETURN(5.6) ),
	    WILL_REPEATEDLY( RETURN(4.8) )
	    );
 // EXPECT_foo(VALUE(7.4f));
  
 // EXPECT_bar(STRING("Hello!"), TIMES(4));

  function(2,3); //Execution: Call the function to test!
  
}
int main(int argc, char **argv)
{
  testhello();
  return 1;
}

