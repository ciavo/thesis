#include "helper.h" 
extern "C" {
  #include <stdarg.h>
#include <string.h>//strcmp()
#include <stdio.h>
#include <stdlib.h>
}
#include <list>
#include <iostream>

using namespace std;
struct Mock_foo{
  int switch0;  double param0; int (*userfunc0)(double x);
  list<Expectation*> list_expectations;
};
list<Mock_foo*> list_foo;

struct Mock_func{
  int switch0;  double param0; int (*userfunc0)(double x);
  int switch1;  int param1; int (*userfunc1)(int x);
  list<Expectation*> list_expectations;
};
list<Mock_func*> list_func;


extern "C"
void expect_foo(int switch0, double param0, int (*userfunc0)(double x), int startvararg, ...){
  printf("INTO: foo \n" );
  struct Mock_foo *obj = new Mock_foo();
  list_foo.push_back(obj);
  obj->switch0= switch0;
  obj->param0= param0;
  obj->userfunc0= userfunc0;


//Add Times Expectations
  if(startvararg == NO_MORE_PARAMETERS ){ // use the default setting
   printf("no more parameters  \n ");
   return;
  }
  va_list ap; 
  va_start(ap, startvararg);
  int time_value;
  if(startvararg == TIMES){
    time_value = va_arg(ap,int);
    struct Expectation* exp = new Expectation(); 
    exp->type_expectation = startvararg; 
    int* p = (int*)malloc(sizeof(int)); 
    *p = time_value;
    exp->returnvalue = (void*)p;
    obj->list_expectations.push_back(exp);
  }else{ 
    time_value = va_arg(ap,int); // to jump the "time_value parameter" used from the TIMES macro 
  }
  //Manage WILLONCE and REPEADETLY
  int type_expectation; // the type of the variable is as the type return function
  double return_value;
  for(type_expectation = va_arg(ap, int); type_expectation != NO_MORE_PARAMETERS; type_expectation = va_arg(ap,int) ){
    struct Expectation* exp = new Expectation(); 
    return_value = va_arg(ap, double);// the return type is always as the return type of the orginal function. We are treatting just WILLONCE and REPEADETLY 
    double* p = (double*)malloc(sizeof(double));
    *p = return_value;
    exp->type_expectation = type_expectation; 
    exp->returnvalue = (void*)p; 
    obj->list_expectations.push_back(exp);
  }
  va_end(ap);
}

extern "C"
void expect_func(int switch0, double param0, int (*userfunc0)(double x), int switch1, int param1, int (*userfunc1)(int x), int startvararg, ...){
  printf("INTO: func \n" );
  struct Mock_func *obj = new Mock_func();
  list_func.push_back(obj);
  obj->switch0= switch0;
  obj->param0= param0;
  obj->userfunc0= userfunc0;

  obj->switch1= switch1;
  obj->param1= param1;
  obj->userfunc1= userfunc1;


//Add Times Expectations
  if(startvararg == NO_MORE_PARAMETERS ){ // use the default setting
   printf("no more parameters  \n ");
   return;
  }
  va_list ap; 
  va_start(ap, startvararg);
  int time_value;
  if(startvararg == TIMES){
    time_value = va_arg(ap,int);
    struct Expectation* exp = new Expectation(); 
    exp->type_expectation = startvararg; 
    int* p = (int*)malloc(sizeof(int)); 
    *p = time_value;
    exp->returnvalue = (void*)p;
    obj->list_expectations.push_back(exp);
  }else{ 
    time_value = va_arg(ap,int); // to jump the "time_value parameter" used from the TIMES macro 
  }
  //Manage WILLONCE and REPEADETLY
  int type_expectation; // the type of the variable is as the type return function
  int return_value;
  for(type_expectation = va_arg(ap, int); type_expectation != NO_MORE_PARAMETERS; type_expectation = va_arg(ap,int) ){
    struct Expectation* exp = new Expectation(); 
    return_value = va_arg(ap, int);// the return type is always as the return type of the orginal function. We are treatting just WILLONCE and REPEADETLY 
    int* p = (int*)malloc(sizeof(int));
    *p = return_value;
    exp->type_expectation = type_expectation; 
    exp->returnvalue = (void*)p; 
    obj->list_expectations.push_back(exp);
  }
  va_end(ap);
}

extern "C" 
double foo(double param0){
 // check expectations 
  struct Mock_foo*obj  = list_foo.front();
  int ris;
  void* return_value;
  bool check_parameter = true;
  switch( obj->switch0 ){
    case ANY_VALUE: 
      break; 
    case CHECK_PARAMS_VALUE: 
      if( obj->param0 != param0 ){ 
         manage_errors(DIFFERENT_PARAMETER, "foo"); 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc0(param0);
      if( ris !=0 ){ 
         manage_errors(DIFFERENT_PARAMETER, "foo"); 
         check_parameter = false;
      }
      break; 
    default: 
      break; 
  }//close switch 
  if(check_parameter == true){
        return_value = check_expectations(obj->list_expectations, "foo");
        if(return_value == NULL){return NULL;} //Error in check_expectations
  }
  double casting_return_value = *(double*) return_value;
  return casting_return_value;

}

extern "C" 
int func(double param0, int param1){
 // check expectations 
  struct Mock_func*obj  = list_func.front();
  int ris;
  void* return_value;
  bool check_parameter = true;
  switch( obj->switch0 ){
    case ANY_VALUE: 
      break; 
    case CHECK_PARAMS_VALUE: 
      if( obj->param0 != param0 ){ 
         manage_errors(DIFFERENT_PARAMETER, "func"); 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc0(param0);
      if( ris !=0 ){ 
         manage_errors(DIFFERENT_PARAMETER, "func"); 
         check_parameter = false;
      }
      break; 
    default: 
      break; 
  }//close switch 
  switch( obj->switch1 ){
    case ANY_VALUE: 
      break; 
    case CHECK_PARAMS_VALUE: 
      if( obj->param1 != param1 ){ 
         manage_errors(DIFFERENT_PARAMETER, "func"); 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc1(param1);
      if( ris !=0 ){ 
         manage_errors(DIFFERENT_PARAMETER, "func"); 
         check_parameter = false;
      }
      break; 
    default: 
      break; 
  }//close switch 
  if(check_parameter == true){
        return_value = check_expectations(obj->list_expectations, "func");
        if(return_value == NULL){return NULL;} //Error in check_expectations
  }
  int casting_return_value = *(int*) return_value;
  return casting_return_value;

}

