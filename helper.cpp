#include <iostream>
#include "helper.h"

//using namespace std;
void manage_errors(int  error_type, string function_name){
  cout << "=== TEST FAIL ===" << endl;
  switch(error_type){
  case DIFFERENT_PARAMETER :
     cout << "From "<< function_name << endl;
     cout << "Parameters do not matching "<< endl;
     break;
  default:
    break;
  }
  cout << "=================" << endl;

}
