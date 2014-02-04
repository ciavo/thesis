#include <stdio.h>

#include "myframework.h"

//#include "temporary_user_structs.h"
//#include "system_under_test.h"
#include "temporary_file.h" //import MACROs for the expectation functions


void test_order_fillingRemovesInventoryIfInStock(){

  EXPECT_warehouse_remove(STRING("Wine"),
	                  VALUE(50),
	                  TIMES(1)	    
	                 );
  
  EXPECT_warehouse_has_inventory(STRING("Wine"),
	               VALUE(50),
	               TIMES(1),
	               WILL_ONCE(RETURN(0) )	    
	              );
  fill("Wine", 50); //Execution: Call the function to test!

}
int main(int argc, char **argv)
{
  
  test_order_fillingRemovesInventoryIfInStock();
  TEST_FINISHED;

  if(isFilled() != 1){
    printf("Test falited\n");
  }else{
    printf("Test PASSED\n");
  }
  return 1;
}
