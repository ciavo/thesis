#include "helper.h" 
extern "C" {
  #include <stdarg.h>
#include <string.h>//strcmp()
#include <stdio.h>
#include <stdlib.h>
}
#include <sstream>
#include <list>
#include <iostream>
#include "user_headers.h"

using namespace std;
struct Mock_warehouse_has_inventory{
  int switch0;  char* param0; int (*userfunc0)(char* x);
  int switch1;  int param1; int (*userfunc1)(int x);
  list<Expectation*> list_expectations;
};
list<Mock_warehouse_has_inventory*> list_warehouse_has_inventory;

struct Mock_warehouse_remove{
  int switch0;  char* param0; int (*userfunc0)(char* x);
  int switch1;  int param1; int (*userfunc1)(int x);
  list<Expectation*> list_expectations;
};
list<Mock_warehouse_remove*> list_warehouse_remove;


extern "C"
void expect_warehouse_has_inventory(int switch0, char* param0, int (*userfunc0)(char* x),
int switch1, int param1, int (*userfunc1)(int x),
int startvararg, ...){
  printf("INTO: warehouse_has_inventory \n" );
  struct Mock_warehouse_has_inventory *obj = new Mock_warehouse_has_inventory();
  list_warehouse_has_inventory.push_front(obj);
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
  if(startvararg == TIMES || startvararg == TIMES_AT_LEAST || startvararg == TIMES_AT_MOST){
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
void expect_warehouse_remove(int switch0, char* param0, int (*userfunc0)(char* x),
int switch1, int param1, int (*userfunc1)(int x),
int startvararg, ...){
  printf("INTO: warehouse_remove \n" );
  struct Mock_warehouse_remove *obj = new Mock_warehouse_remove();
  list_warehouse_remove.push_front(obj);
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
  if(startvararg == TIMES || startvararg == TIMES_AT_LEAST || startvararg == TIMES_AT_MOST){
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
  va_end(ap);
}

extern "C" 
int warehouse_has_inventory(char* param0, int param1){
 // check expectations 
  int ris;
  void* return_value;
  bool check_parameter = false;
  Mock_warehouse_has_inventory *obj ;
  for (list<Mock_warehouse_has_inventory*>::iterator it = list_warehouse_has_inventory.begin(); it!= list_warehouse_has_inventory.end() &&  !check_parameter ; ++it){
  check_parameter = true;
  obj = *it;
  switch( obj->switch0 ){
    case ANY_VALUE: 
      break; 
    case CHECK_PARAMS_VALUE: 
      if( obj->param0 != param0 ){ 
         check_parameter = false;
      }
      break; 
    case Gt: 
      if( obj->param0 >= param0 ){ 
         check_parameter = false;
      }
      break; 
    case Lt: 
      if( obj->param0 <= param0 ){ 
         check_parameter = false;
      }
      break; 
    case Ge: 
      if( obj->param0 > param0 ){ 
         check_parameter = false;
      }
      break; 
    case Le: 
      if( obj->param0 < param0 ){ 
         check_parameter = false;
      }
      break; 
    case Ne: 
      if( obj->param0 == param0 ){ 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc0(param0);
      if( ris !=0 ){ 
         check_parameter = false;
      }
      break; 
    case CHECK_PARAMS_STRING: 
      if( strcmp(obj->param0, param0)!=0 ){ 
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
         check_parameter = false;
      }
      break; 
    case Gt: 
      if( obj->param1 >= param1 ){ 
         check_parameter = false;
      }
      break; 
    case Lt: 
      if( obj->param1 <= param1 ){ 
         check_parameter = false;
      }
      break; 
    case Ge: 
      if( obj->param1 > param1 ){ 
         check_parameter = false;
      }
      break; 
    case Le: 
      if( obj->param1 < param1 ){ 
         check_parameter = false;
      }
      break; 
    case Ne: 
      if( obj->param1 == param1 ){ 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc1(param1);
      if( ris !=0 ){ 
         check_parameter = false;
      }
      break; 
    default: 
      break; 
  }//close switch 
}//close for
  if(check_parameter == true){
    return_value = check_expectations(obj->list_expectations, "warehouse_has_inventory");
  }else{
     manage_errors(DIFFERENT_PARAMETER, "warehouse_has_inventory");
    return_value = NULL;
}
  if(return_value == NULL){return NULL;} //Error in check_expectations
  int casting_return_value = *(int*) return_value;
  return casting_return_value;

}

extern "C" 
void warehouse_remove(char* param0, int param1){
 // check expectations 
  int ris;
  void* return_value;
  bool check_parameter = false;
  Mock_warehouse_remove *obj ;
  for (list<Mock_warehouse_remove*>::iterator it = list_warehouse_remove.begin(); it!= list_warehouse_remove.end() &&  !check_parameter ; ++it){
  check_parameter = true;
  obj = *it;
  switch( obj->switch0 ){
    case ANY_VALUE: 
      break; 
    case CHECK_PARAMS_VALUE: 
      if( obj->param0 != param0 ){ 
         check_parameter = false;
      }
      break; 
    case Gt: 
      if( obj->param0 >= param0 ){ 
         check_parameter = false;
      }
      break; 
    case Lt: 
      if( obj->param0 <= param0 ){ 
         check_parameter = false;
      }
      break; 
    case Ge: 
      if( obj->param0 > param0 ){ 
         check_parameter = false;
      }
      break; 
    case Le: 
      if( obj->param0 < param0 ){ 
         check_parameter = false;
      }
      break; 
    case Ne: 
      if( obj->param0 == param0 ){ 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc0(param0);
      if( ris !=0 ){ 
         check_parameter = false;
      }
      break; 
    case CHECK_PARAMS_STRING: 
      if( strcmp(obj->param0, param0)!=0 ){ 
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
         check_parameter = false;
      }
      break; 
    case Gt: 
      if( obj->param1 >= param1 ){ 
         check_parameter = false;
      }
      break; 
    case Lt: 
      if( obj->param1 <= param1 ){ 
         check_parameter = false;
      }
      break; 
    case Ge: 
      if( obj->param1 > param1 ){ 
         check_parameter = false;
      }
      break; 
    case Le: 
      if( obj->param1 < param1 ){ 
         check_parameter = false;
      }
      break; 
    case Ne: 
      if( obj->param1 == param1 ){ 
         check_parameter = false;
      }
      break; 
    case USER_FUNCTION: 
      ris = obj->userfunc1(param1);
      if( ris !=0 ){ 
         check_parameter = false;
      }
      break; 
    default: 
      break; 
  }//close switch 
}//close for
  if(check_parameter == true){
    return_value = check_expectations(obj->list_expectations, "warehouse_remove");
  }else{
     manage_errors(DIFFERENT_PARAMETER, "warehouse_remove");
    return_value = NULL;
}

}


//When the test is finished we have to see if all the expectations have been respected or not.
extern "C" 
void testCompleted(){
  int expectation_number = 1 ;

  for ( list<Mock_warehouse_has_inventory*>::iterator it = list_warehouse_has_inventory.begin(); it!= list_warehouse_has_inventory.end() ;expectation_number++, ++it){
    struct Mock_warehouse_has_inventory* obj_warehouse_has_inventory = *it;
    std::stringstream ss ;
    ss << "warehouse_has_inventory" << " from Expectation number: "<< expectation_number; 
    check_expectations_test_finished(obj_warehouse_has_inventory->list_expectations , ss.str() );
  }

   expectation_number = 1;
  for ( list<Mock_warehouse_remove*>::iterator it = list_warehouse_remove.begin(); it!= list_warehouse_remove.end() ;expectation_number++, ++it){
    struct Mock_warehouse_remove* obj_warehouse_remove = *it;
    std::stringstream ss ;
    ss << "warehouse_remove" << " from Expectation number: "<< expectation_number; 
    check_expectations_test_finished(obj_warehouse_remove->list_expectations , ss.str() );
  }

   expectation_number = 1;
}
