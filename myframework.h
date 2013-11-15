#ifndef MYFRAMEWORK_H
#define MYFRAMEWORK_H



#define VALUE(x)         0, x, NULL
#define ANY_VALUE        1, 0, NULL

// STRING(x) manage char* types 
#define STRING(x)        2, x, NULL

//USER_FUNCTION(x) x: (*function)() The function return 0 if the test is postive. Otherwise the test will fail.
#define USER_FUNCTION(x) 3, 0, x 

//y is the parameter  got from the mocked function call
#define Gt(x)            4, x, NULL    //  x > y
#define Lt(x)            5, x, NULL    //  x < y
#define Ge(x)            6, x, NULL    //  x >= y
#define Le(x)            7, x, NULL    //  x <= y
#define Ne(x)            8, x, NULL    //  x != y

#define TIMES(x)           200, x
#define WILLONCE(x)        201, x
#define WILL_REPEATEDLY(x) 202, x
#define RETURN(x)          x



#define NO_MORE_PARAMETERS -256


#endif
