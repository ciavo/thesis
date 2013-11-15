#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <list>
#include <string>

using namespace std;

struct Expectation {
  int type_expectation; //TIMES, WILLONCE, REPEADETLY
  void* returnvalue;
  int number_calls ; // TIMES use to keep trace of the number of times that we call the corrispondent function. 
  Expectation(){
    number_calls = 0; 
  }
};

//VALUE(x)
#define CHECK_PARAMS_VALUE  0
#define ANY_VALUE           1
//STRING(x)
#define CHECK_PARAMS_STRING 2
/*    USER_FUNCTION(x)   x is the address of the function to use. Return 0 if the test is positive. Otherwise the test will fail. 
*/
//USER_FUNCTION(x)
#define USER_FUNCTION       3

#define Gt                  4
#define Lt                  5
#define Ge                  6
#define Le                  7
#define Ne                  8



// ===== End Generic Comparison =======

// ===== Errors ===============

#define DIFFERENT_PARAMETER       100
#define TIMES_CALLS_EXCEEDED      101
#define Gt_ERROR                  102
#define Lt_ERROR                  103
#define Ge_ERROR                  104
#define Le_ERROR                  105
#define Ne_ERROR                  106

// ===== End Errors ============

/*========== Expectations ===================== 
    .With(multi_argument_matcher)               No
    .Times(cardinality)                         Yes
    .InSequence(sequences)                      No
    .After(expectations)                        No
    .WillOnce(action)                           Yes
    .WillRepeatedly(action)                     Yes
    .RetiresOnSaturation();                     No
 */
#define TIMES           200
#define WILLONCE        201
#define WILL_REPEATEDLY  202
#define NO_MORE_PARAMETERS -256


// ====== END Actions definitions ===================


void* check_expectations(list<Expectation*>& list_exp, string function_name);
void check_expectations_test_finished(list<Expectation*>& list_exp, string function_name);



using namespace std;
void manage_errors(int  error_type, string function_name);

#endif
