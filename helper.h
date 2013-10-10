#ifndef HELPER_H
#define HELPER_H

//VALUE(x)
#define CHECK_PARAMS_VALUE 0

#define ANY_VALUE 1

//STRING(x)
#define CHECK_PARAMS_STRING 2

//USER_FUNCTION(x,y)
#define USER_FUNCTION 3

//==== Errors ===
#define DIFFERENT_PARAMETER 100



#include <string>
//void manage_errors(int error);
using namespace std;
void manage_errors(int  error_type, string function_name);

#endif
