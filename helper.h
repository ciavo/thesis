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
#define CHECK_PARAMS_VALUE 0
#define ANY_VALUE 1
//STRING(x)
#define CHECK_PARAMS_STRING 2



/* ======================================
  Generic Comparison

No     Eq(value) or value	argument == value
No     Ge(value)	argument >= value
No     Gt(value)	argument > value
No     Le(value)	argument <= value
No     Lt(value)	argument < value
No     Ne(value)	argument != value
No     IsNull()	argument is a NULL pointer (raw or smart).
No     NotNull()	argument is a non-null pointer (raw or smart).
No     Ref(variable)	argument is a reference to variable.
No     TypedEq<type>(value)	argument has type type and is equal to value. You may need to use this instead of Eq(value) when the mock function is overloaded.

Yes    USER_FUNCTION(x)   x is the address of the function to use. Return 0 if the test is positive. Otherwise the test will fail. 
*/

//USER_FUNCTION(x)
#define USER_FUNCTION 3


// ===== End Generic Comparison =======

// ===== Errors ===============

#define DIFFERENT_PARAMETER       100
#define TIMES_CALLS_EXCEEDED      101
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



using namespace std;
void manage_errors(int  error_type, string function_name);

#endif
