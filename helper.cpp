#include "helper.h"

//using namespace std;
void manage_errors(int  error_type, string function_name){
  cout << "=== TEST FAIL ===" << endl;
  switch(error_type){
  case DIFFERENT_PARAMETER :
     cout << "From "<< function_name << endl;
     cout << "Parameters do not matching "<< endl;
     break;
  case  TIMES_CALLS_EXCEEDED:
     cout << "From "<< function_name << endl;
     cout << "Time calls exceeded! "<< endl;
  default:
    break;
  }
  cout << "=================" << endl;

}

void* check_expectations(list<Expectation*>& list_exp, string function_name){
  if( list_exp.size() == 0 ){
    return NULL;
  }

  //The first expectation it has to be always TIMES
  list<Expectation*>::iterator it_exp = list_exp.begin();
  Expectation* exp_times = *it_exp;
  int total_number_calls = *(int*)exp_times->returnvalue;
  ++it_exp;
  exp_times->number_calls ++;  
  if(exp_times->number_calls > total_number_calls){
    manage_errors( TIMES_CALLS_EXCEEDED, function_name);
    return NULL;
  }

  

  // a function that return void will not flow this loop.
  for (int x = 1; it_exp != list_exp.end(); ++it_exp, x ++){
    switch((*it_exp)->type_expectation){
      case WILLONCE:
	if(x == exp_times->number_calls){
	  return (*it_exp)->returnvalue;
	}
	break;
      
      case WILL_REPEATEDLY:
        return (*it_exp)->returnvalue;
	break;    
    }
  }
  return NULL;
}
