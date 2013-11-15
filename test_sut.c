#include <stdio.h>

#include "myframework.h"
#include "temporary_file.h"


void test_sut(){

  EXPECT_foo(VALUE(6),
	    TIMES(1),
	    WILLONCE( RETURN(7) )	    
	    );
 

  function_to_test(); //Execution: Call the function to test!

}
int main(int argc, char **argv)
{
  test_sut();

  return 1;
}

