#include "helper.h"

//using namespace std;
void manage_errors(int  error_type, string function_name){
  cout << endl;
  cout << endl;
  cout << "=== TEST FAIL ===" << endl;
  switch(error_type){
  case DIFFERENT_PARAMETER :
     cout << "From "<< function_name << endl;
     cout << "Parameters do not matching "<< endl;
     break;
  
  case  TIMES_CALLS_EXCEEDED:
     cout << "From "<< function_name << endl;
     cout << "Time calls exceeded! "<< endl;
     break;
  
  case Gt_ERROR:
     cout << "From "<< function_name << endl;
     cout << "The value received from the function called is not GREATER of one set in the expectation "<< endl;
     break;
  
  case  Lt_ERROR:
     cout << "From "<< function_name << endl;
     cout << "The value received from the function called is not LESS of one set in the expectation "<< endl;
     break;
  
  case  Le_ERROR:
     cout << "From "<< function_name << endl;
     cout << "The value received from the function called is not LESS or EQUAL of one set in the expectation "<< endl;
     break;

  case  Ge_ERROR:
     cout << "From "<< function_name << endl;
     cout << "The value received from the function called is not GREATER or EQUAL of one set in the expectation "<< endl;
     break;
  
  case  Ne_ERROR:
     cout << "From "<< function_name << endl;
     cout << "The value received from the function called is not EQUAL of one set in the expectation "<< endl;
     break;
  
  default:
    break;
  }
  cout << "=================" << endl;
  cout << endl;
}

void manage_times_errors(int  error_type, string function_name, int total_calls, int made_calls){
  cout << "=== TEST FAIL ===" << endl;
  switch(error_type){

    case TIMES_CALLS_LESSER:
      cout << "The function \""<< function_name << "\" should be called AT LEAST " << total_calls;
      cout << (total_calls > 1? " times" : " time") << endl;
      cout << "but it is called just " << made_calls << (made_calls > 1? " times":" time") << endl;
      break;

    case TIMES_CALLS_NEQUAL:
      cout << "The function \""<< function_name << "\" should be called EXACTLY " << total_calls;
      cout << (total_calls > 1? " times" : " time") << endl;
      cout << "but it is called just " << made_calls << (made_calls > 1? " times":" time") << endl;
      break;
    
    case TIMES_CALLS_MORE:
      cout << "The function \""<< function_name << "\" should be called AT MOST " << total_calls;
      cout << (total_calls > 1? " times" : " time") << endl;
      cout << "but it is called just " << made_calls << (made_calls > 1? " times":" time") << endl;
      break;
   
  default:
    break;
  }
  cout << "=================" << endl;
}
void check_expectations_test_finished(list<Expectation*>& list_exp, string function_name){
  
  //The first expectation it has to be always TIMES or TIME_AT_MOST or TIME_AT_LEAST
  list<Expectation*>::iterator it_exp = list_exp.begin();
  Expectation* exp_times = *it_exp;
  int total_number_calls = *(int*)exp_times->returnvalue;
  
//we need to check just if the number calls made are less then the total_number_calls setted in the expectation because the check for the "greater" case is done directly in the "check_expectations" function.

  switch(exp_times->type_expectation){
  case TIMES: 
    if( exp_times->number_calls != total_number_calls ){
      manage_times_errors(TIMES_CALLS_NEQUAL , function_name, total_number_calls, exp_times->number_calls );
    }
    break;
    
  case TIMES_AT_LEAST:
    if( exp_times->number_calls < total_number_calls ){
      manage_times_errors(TIMES_CALLS_LESSER , function_name, total_number_calls, exp_times->number_calls );
    }
  break;
  
  case TIMES_AT_MOST: 
    if( exp_times->number_calls > total_number_calls ){
      manage_times_errors(TIMES_CALLS_MORE , function_name, total_number_calls, exp_times->number_calls );
    }
  default:
    break;    
  }
  return;
}

void* check_expectations(list<Expectation*>& list_exp, string function_name){
  if( list_exp.size() == 0 ){
    return NULL;
  }
  //The first expectation it has to be always TIMES, TIMES_AT_LEAST or TIMES_AT_MOST
  list<Expectation*>::iterator it_exp = list_exp.begin();
  Expectation* exp_times = *it_exp;
  int total_number_calls = *(int*)exp_times->returnvalue;
  ++it_exp;
  exp_times->number_calls ++;  
  if(exp_times->number_calls > total_number_calls){
    manage_errors( TIMES_CALLS_EXCEEDED, function_name);
    return NULL;
  }
  // a mock function corrisponding at a SUT eternal function that return void will not flow this loop.
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
