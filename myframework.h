
#define ANY_VALUE 1, 0, NULL
#define VALUE(x) 0, x, NULL

// STRING(x) manage char* types 
#define STRING(x) 2, x, NULL

//USER_FUNCTION(x,y) x: *function, y: parameter to the function. The function return 0 if the test is postive. Otherwise the test will fail.
#define USER_FUNCTION(x,y) 3, y, x 

//#include "temporary_file.h"
