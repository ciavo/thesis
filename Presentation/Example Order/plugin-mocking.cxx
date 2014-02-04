// GCC header includes to get the parse tree declarations. The order
// is important and doesn't follow any kind of logic.
//

//#include <stdlib.h>
#include <gmp.h>
#include <cstdlib> // Include before GCC poison some declarations.


#include "gcc-plugin.h"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-iterator.h"
#include "intl.h"

#include "tm.h"

#include "diagnostic.h"
  //#include "c-common.h"
  //#include "c-pragma.h"
  //#include "cp/cp-tree.h"

#include <set>


//my inclusions
#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

int plugin_is_GPL_compatible;
 

list<tree_node*> list_functions_with_only_declaration;
list<tree_node*> list_functions_with_body;
list<tree_node*> list_functions_to_mock;

ofstream myfile;      // used to write into the temporary_file.cpp
ofstream headerfile;  // used to write into the temporary_file.h
ofstream userStructDefined_file;
int function_type_number = 0;

string manage_types(tree , bool , bool);

void print_node_declaration(tree_node* node_declaration){
  int treecode = TREE_CODE(node_declaration);
  tree id = DECL_NAME(node_declaration);
  string source_file = DECL_SOURCE_FILE(node_declaration);
  int number_line =  DECL_SOURCE_LINE(node_declaration);
  const char* name (id ? IDENTIFIER_POINTER (id): "<unnamed>");
  cout << "*** Tree node:"  << tree_code_name[treecode]  << " Function name: " << name << " in "
       <<  source_file <<"  number line: " << number_line << " ***" << endl;
}



void cb_finish_decl(void *gcc_data, void *user_data){
  //cout << "****** EVENT PLUGIN_DECLARATION *******" << endl;
  tree_node *ast_node = reinterpret_cast<tree_node*>(gcc_data);
  int treecode = TREE_CODE(ast_node);
  
  //  print_node_declaration(ast_node);

  if(treecode == FUNCTION_DECL){// we are interested only to the function that are rapresented in the AST as a FUNCTION_DECL node
    string source_file = DECL_SOURCE_FILE(ast_node);
    size_t found = source_file.find("/usr/include");
    if(found==string::npos ){// we are NOT interested to mock library functions.
      print_node_declaration(ast_node);
      //      list_functions_to_mock.push_back(ast_node);
      list_functions_with_only_declaration.push_back(ast_node);
    }
  }
}

// ============================================================================
/* 
     We save all the functions that have a body into the list:  list_functions_with_body  
 */
void cb_plugin_pre_generize (void *maintree, void*)
{
    
  cout <<  "************** PLUGIN_PRE_GENERIZE EVENT *******************"<< endl;  
  tree_node *tn = reinterpret_cast<tree_node*>(maintree);
  cout << " --- Function name: " << IDENTIFIER_POINTER(DECL_NAME(tn)) << " ---" << endl;
  // print_node_declaration(tn);
  int treecode = TREE_CODE(tn);
  //if(treecode == FUNCTION_DECL){
      list_functions_with_body.push_back(tn);
    //}
  cout << "**************** Ends PLUGIN_PRE_GENERIZE EVENT *************" << endl;
}

// ===================================================================




void print_list(const list<tree_node*>& list_){
 
  for ( list<tree_node*>::const_iterator it = list_.begin(); it != list_.end(); ++it){
    print_node_declaration(*it);
  }

}

string manage_enumeral_type(tree node, bool isPartial){
  cout << "== manage enumeral type ===" << endl;
  string name_enumeration;
  std::stringstream user_enumeration;
  
  if (TYPE_NAME (node)){
    
    if (TREE_CODE (TYPE_NAME (node)) == IDENTIFIER_NODE){
      name_enumeration = IDENTIFIER_POINTER (TYPE_NAME (node));
    }
    
    else if (TREE_CODE (TYPE_NAME (node)) == TYPE_DECL
	     && DECL_NAME (TYPE_NAME (node))){
      name_enumeration = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (node)));
    }
  }
  cout << " the name of the enumeration is: " << name_enumeration << endl;
  //  debug_tree(node);  
  
  if(isPartial){
    return name_enumeration;
  }

  user_enumeration << "typedef enum \n{" << endl;
  tree node_list = TYPE_VALUES(node);

  while(node_list != NULL){
    user_enumeration <<  IDENTIFIER_POINTER(TREE_PURPOSE(node_list)) ;
    // cout  << IDENTIFIER_POINTER(TREE_PURPOSE(node_list)) << ";" << endl;
    node_list = TREE_CHAIN(node_list);
    if(node_list!=NULL){
      user_enumeration << "," << endl;
    }else{
      user_enumeration << endl;
    }
  }
  
  user_enumeration << "}" << name_enumeration <<";" << endl;
  //cout << "complete enumeration: \n" << user_enumeration <<  endl;
  //cout << "print the enumeration: "<< user_enumeration.str() <<  endl;  
  return user_enumeration.str();
}

string manage_record_type(tree node, bool isPartial) // if ispartial == true then return only the "struc nameofthestruct"
{
  //string user_struct = "struct ";
  string name_struct;

  std::stringstream user_struct;
  debug_tree(node);
  if (TYPE_NAME (node)){
    if (TREE_CODE (TYPE_NAME (node)) == IDENTIFIER_NODE){
      name_struct = IDENTIFIER_POINTER (TYPE_NAME (node));
    }
    else if (TREE_CODE (TYPE_NAME (node)) == TYPE_DECL
	     && DECL_NAME (TYPE_NAME (node))){
            name_struct = IDENTIFIER_POINTER (DECL_NAME (TYPE_NAME (node)));
    }
  }
  cout << " the name of the struct is: " << name_struct << endl;
  //user_struct = user_struct + name_struct + "{\n" ;
  if(isPartial){
    user_struct << "struct "<< name_struct ;
    return user_struct.str();
  }

  user_struct << "struct " << name_struct << "{" << endl;
  //Navigate struct's type fields.
  tree node_field_chain = TYPE_FIELDS(node);
  tree field;
  int number_param = 0;
   while(node_field_chain !=NULL ){
     field = TREE_TYPE(node_field_chain);
     //debug_tree(field);

     user_struct <<"  "<< manage_types(field,false,false) << " param" << number_param << ";" << endl;

     node_field_chain = TREE_CHAIN(node_field_chain);
     number_param ++;
   }
   user_struct << "};" <<  endl;
   //cout << "complete struct: \n" << user_struct <<  endl;
   cout <<"print the struct: "<< user_struct.str() <<  endl;  
   return user_struct.str();
    
}


string manage_function_type(tree functiontypenode, int count){
  /* 
source: http://www.delorie.com/gnu/docs/gcc/gccint_31.html

The TREE_TYPE gives the return type of the function. The TYPE_ARG_TYPES are a TREE_LIST of the argument types. 
The TREE_VALUE of each node in this list is the type of the corresponding argument; 
the TREE_PURPOSE is an expression for the default argument value, if any. 
If the last node in the list is void_list_node (a TREE_LIST node whose TREE_VALUE is the void_type_node), 
then functions of this type do not take variable arguments. Otherwise, they do take a variable number of arguments.

Note that in C (but not in C++) a function declared like void f() is an unprototyped function taking a variable number of arguments; the TYPE_ARG_TYPES of such a function will be NULL.   

  */

  cout << "Into manage_function_type "<< endl;
  //cout << manage_types(TREE_TYPE(functiontypenode), false, false);  
  
  std::stringstream functionss;
  functionss << manage_types(TREE_TYPE(functiontypenode), false, false);
  cout << functionss.str()<< endl;
  functionss << " (* param"<< count <<")(";
  cout << functionss.str()<< endl;
  tree node = TYPE_ARG_TYPES(functiontypenode);
  
  int number_parameter = 0;
  if(node!=NULL){
    do{     
      functionss << manage_types(TREE_VALUE(node),false,true);// << " x" << number_parameter;  //for each arguments: get the type argument and put it into the list_type_args
       if(TREE_CHAIN(node) != NULL){
         functionss << ", ";
       }
    }
    while(node = TREE_CHAIN(node) );
  }
  functionss << ")";

  cout <<"the function printed is: " << functionss.str()<< endl;
  
  return functionss.str();  
}

string  manage_types(tree nodetype, bool isPuntator, bool isPartial)
{
  string result;
  cout << " *** Into manage_types() ***" << endl;
   debug_tree(nodetype);
  if(isPuntator)
    cout << "PUNTATOR to an: "<< endl;
  //debug_tree(nodetype);
   //  cout << "Node Type: " <<  tree_code_name[TREE_CODE(nodetype)] << endl;
  switch(TREE_CODE(nodetype)){
   case INTEGER_TYPE: 
     if( TYPE_STRING_FLAG(nodetype) == 1 ){
       result = "char";
       //   cout <<" Type: char " <<  endl;
     }else{
       //cout <<" TYPE_PRECISION:  " << TYPE_PRECISION(nodetype)  << endl;
       switch( TYPE_PRECISION(nodetype)){
        case 16 :
	  result = "short";
	  // cout << "type: short"<< endl;
          break; 
        case 32 :
	   result = "int";
	  //cout << "type: int"<< endl;
          break; 
        case 64 :
	   result = "long";
	  //cout << "type: long"<< endl;
          break; 
       }
     }
     break;

  case REAL_TYPE:
    // cout <<" TYPE_PRECISION:  " << TYPE_PRECISION(nodetype)  << endl;
    switch( TYPE_PRECISION(nodetype)){
      case 32 :
	 result = "float";
	//	cout << "type: float"<< endl;
	break; 
      case 64 :
	 result = "double";
	//	cout << "type: double"<< endl;
	break; 
      case 80:
	  result = "long double";
	 //	cout << "type: long double" << endl;
    }
    break;

  case VOID_TYPE:
     result = "void";
    break;
  case COMPLEX_TYPE:
    break;
  case ENUMERAL_TYPE:
    if(isPartial){
      result = manage_enumeral_type(nodetype, true);
    }else{
      result = manage_enumeral_type(nodetype, false);
    }
    break;
  case BOOLEAN_TYPE:
    break;
     case FUNCTION_TYPE:
     cout << "Into FUNCTION_TYPE" << endl;
     result = manage_function_type(nodetype,0);
     break;
    
  case POINTER_TYPE:
     cout << "Into POINTER_TYPE" << endl;
    // debug_tree(nodetype);
    //cout << "==================" << endl;
    //debug_tree(TREE_TYPE(nodetype));
    // It is really important give true to the 3rd parameter to manage the struct list. Otherwise it will be an infinite loop
    
     if(TREE_CODE(TREE_TYPE(nodetype)) == FUNCTION_TYPE){
       result = manage_types(TREE_TYPE(nodetype), false, false);
     }else{
       result = manage_types(TREE_TYPE(nodetype), true, true)+"*";
     }
    break;
  case RECORD_TYPE:
    //cout << "Into RECORD_TYPE" << endl;
    // debug_tree(nodetype);
    cout << "==================" << endl;
    
    if(isPartial){
      result = manage_record_type(nodetype, true);
    }else{
      result = manage_record_type(nodetype, false);
    }
    break;
  default:
    // result ="void";
     break;
  }
   cout << endl;
   cout << "Into Manage types. result= "<< result << endl;
   cout << endl;
  return result;
}



void write_external_functions(){
 for (list<tree_node*>::iterator it = list_functions_to_mock.begin(); it != list_functions_to_mock.end(); ++it){
    list<string> list_type_args;
    list<tree_node*> list_node_args;
    cout << endl;
    print_node_declaration(*it);
    cout << endl;
    // debug_tree(*it);  
    cout << endl;
    string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it));
    cout << "Writing mock function: " << function_name << endl;
    // cout << "Function name:" << function_name<< endl;
    //cout << "---GET THE RETURN TYPE---"<< endl;
    tree_node *tn =  reinterpret_cast<tree_node*>(*it);
    tree get_decl_result = DECL_RESULT(tn);
   
    // cout << "debug 1.2. ----  1 means that the function is external:  " << DECL_EXTERNAL(*it) <<  endl;
   
    string return_type =  manage_types(TREE_TYPE( TREE_TYPE(*it) ),false, true);
    myfile << "extern \"C\" "<<endl;
    myfile << return_type << " " << function_name << "("; 
  
    cout << endl;
  
    //cout << "take arguments " << endl;
    tree node = TYPE_ARG_TYPES( TREE_TYPE(*it) );
    int count = 0;
    if(node!=NULL){
      do{
	list_type_args.push_back( manage_types(TREE_VALUE(node),false,true));//for each arguments: get the type argument and put it into the list_type_args
	list_node_args.push_back(TREE_VALUE(node));
      }
      while(node = TREE_CHAIN(node) );

      cout << endl;
      // cout << "Print type arguments"<<endl;
      //print type arguments
     
      
     for (list<tree_node*>::iterator x = list_node_args.begin();count < list_node_args.size()-1 ; ++x, count++){

	if(TREE_CODE(*x) == POINTER_TYPE){
	  if(TREE_CODE(TREE_TYPE(*x)) == FUNCTION_TYPE){
	    myfile << manage_function_type(TREE_TYPE(*x),count) ;
	  }    
	}else if( TREE_CODE(*x) == RECORD_TYPE ){
	  myfile << manage_types(*x,false, true) << "* param" << count ;
	  
	}else{
	  myfile <<  manage_types(*x, false, true) <<  " param" << count ; 
	}
 
	if(count != list_node_args.size()-2){
	  myfile << ", ";
	}
      }
     /*
      //==============
      //we must jump the last element that its always (a void type) not necessary
      for (list<string>::iterator x = list_type_args.begin(); count < list_type_args.size()-1 ; ++x, count++){
      
	myfile << *x << " param" << count;
	if(count != list_type_args.size()-2){
	  myfile << ", ";
	}
      }
      //===================
      */


    }
    myfile << "){" << endl;
    myfile << " // check expectations " << endl;
    // myfile << "  struct Mock_" << function_name << "*obj  = list_"<<function_name << ".front();" << endl;
    myfile << "  int ris;" << endl;
    myfile << "  void* return_value;" << endl;
    myfile << "  bool check_parameter = false;" << endl;
    count = 0;
    myfile << "  Mock_" << function_name  << " *obj ;"<< endl;
    myfile << "  for (list<Mock_"<< function_name  <<"*>::iterator it = list_"<<function_name <<".begin(); it!= list_"<< function_name  <<".end() &&  !check_parameter ; ++it){" << endl;
    myfile << "  check_parameter = true;" << endl;
    myfile << "  obj = *it;" << endl;
    
    for (list<tree_node*>::iterator x = list_node_args.begin();!(list_node_args.empty()) &&  count < list_node_args.size()-1 ; ++x, count++){
      myfile << "  switch( obj->switch"<< count <<" ){"<< endl;
      myfile << "    case ANY_VALUE: "<< endl;      
      myfile << "      break; "<< endl; 
      
      if(!(TREE_CODE(TREE_VALUE(*x)) == RECORD_TYPE)){
	myfile << "    case CHECK_PARAMS_VALUE: "<< endl; 
	myfile << "      if( obj->param" << count << " != param" << count <<" ){ "<< endl; 
	//myfile << "         manage_errors(DIFFERENT_PARAMETER, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;      
	myfile << "      break; "<< endl;

	myfile << "    case Gt: "<< endl; 
	myfile << "      if( obj->param" << count << " >= param" << count <<" ){ "<< endl; 
	//myfile << "         manage_errors(Gt_ERROR, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;      
	myfile << "      break; "<< endl;
	
	
	myfile << "    case Lt: "<< endl; 
	myfile << "      if( obj->param" << count << " <= param" << count <<" ){ "<< endl; 
	//myfile << "         manage_errors(Lt_ERROR, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;      
	myfile << "      break; "<< endl;
	


	myfile << "    case Ge: "<< endl; 
	myfile << "      if( obj->param" << count << " > param" << count <<" ){ "<< endl; 
	//myfile << "         manage_errors(Ge_ERROR, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;      
	myfile << "      break; "<< endl;
	
	
	myfile << "    case Le: "<< endl; 
	myfile << "      if( obj->param" << count << " < param" << count <<" ){ "<< endl; 
	//myfile << "         manage_errors(Le_ERROR, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;      
	myfile << "      break; "<< endl;
	
	
	myfile << "    case Ne: "<< endl; 
	myfile << "      if( obj->param" << count << " == param" << count <<" ){ "<< endl; 
	//myfile << "         manage_errors(Ne_ERROR, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;      
	myfile << "      break; "<< endl;
      }
	
	
      myfile << "    case USER_FUNCTION: "<< endl; 
      myfile << "      ris = obj->userfunc"<< count << "(param" << count << ");" << endl;  
      //myfile << "      printf(\"******* ris = %d \\n \",ris);" << endl;
      myfile << "      if( ris !=0 ){ "<< endl; 
      //myfile << "         manage_errors(DIFFERENT_PARAMETER, \""<<function_name<<"\"); "<< endl;
      myfile << "         check_parameter = false;" << endl;
      myfile << "      }"<< endl;
      myfile << "      break; "<< endl;

      
      string type(manage_types(TREE_VALUE(*x),false,true));
      if(type.compare("char*") == 0){
	myfile << "    case CHECK_PARAMS_STRING: "<< endl; 
	myfile << "      if( strcmp(obj->param" << count << ", param" << count <<")!=0 ){ "<< endl; 
	//myfile << "         manage_errors(DIFFERENT_PARAMETER, \""<<function_name<<"\"); "<< endl; 
	myfile << "         check_parameter = false;" << endl;
	myfile << "      }"<< endl;
	myfile << "      break; "<< endl;
      }

      myfile << "    default: "<< endl; 
      myfile << "      break; "<< endl; 
      myfile << "  }//close switch "<< endl;
    }
    myfile << "}//close for" << endl;
     myfile << "  if(check_parameter == true){" << endl;
     myfile << "    return_value = check_expectations(obj->list_expectations, \"" << function_name << "\");" << endl;
       myfile << "  }else{"<< endl;
       myfile <<"     manage_errors(DIFFERENT_PARAMETER, \""<<function_name<<"\");" << endl;
       myfile << "    return_value = NULL;" << endl;
       myfile << "}" << endl;
     if(return_type.compare("void")!=0 ){
        if(return_type.compare("void")!=0 ){
       myfile << "  if(return_value == NULL){return NULL;} //Error in check_expectations" << endl;
     }
    
       myfile <<"  "<< return_type << " casting_return_value = *("<<return_type<<"*) return_value;" << endl;
	myfile <<"  return casting_return_value;" << endl;
     }
    myfile << endl;
    
    myfile << "}" << endl;
    myfile << endl;  
  }
 

}

void  write_expectation_functions(){

 for (list<tree_node*>::iterator it = list_functions_to_mock.begin(); it != list_functions_to_mock.end(); ++it){
    list<string> list_type_args; 
    list<tree_node*> list_node_args;
    cout << endl;
    string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it));
    cout << "Writing Expectation of the function: "<< function_name << endl;;
    
    tree_node *tn =  reinterpret_cast<tree_node*>(*it);
    tree get_decl_result = DECL_RESULT(tn);   
    string return_type =  manage_types(TREE_TYPE( TREE_TYPE(*it) ), false, true);
    //myfile << return_type << " " << function_name << "("; 
    myfile << "extern \"C\"" << endl;
    myfile << "void expect_" << function_name << "(";
 
 
    tree node = TYPE_ARG_TYPES( TREE_TYPE(*it) );//get function's arguments
    int count = 0;
    if(node != NULL){
      do{
	list_type_args.push_back( manage_types(TREE_VALUE(node), false, true ) );//for each arguments: get the type argument and put it into the list_type_args
	list_node_args.push_back( TREE_VALUE(node));
      }
      while(node = TREE_CHAIN(node) );
      
      //write on temporary_file.c the type arguments
      count = 0;
      //it is necessary jump the last element that its always (a void type) not necessary
      for (list<tree_node*>::iterator x = list_node_args.begin();count < list_node_args.size()-1 ; ++x, count++){

	if(TREE_CODE(*x) == POINTER_TYPE){
	  if(TREE_CODE(TREE_TYPE(*x)) == FUNCTION_TYPE){
	    myfile << "  int switch"<< count << ", " << manage_function_type(TREE_TYPE(*x),count)  << "," << " int (*userfunc" << count << ")("<< manage_function_type(TREE_TYPE(*x), count) << "),"  << endl;
	  }    
	}else if( TREE_CODE(*x) == RECORD_TYPE ){
	  myfile << "int switch"<< count << ", " << manage_types(*x,false, true) << "* param" << count << ", int (*userfunc"<<count<<")( "<<manage_types(*x, false, true) << " x)," << endl;
	  
	}else{
	  myfile << "int switch"<< count << ", " << manage_types(*x, false, true) << " param" << count << ", int (*userfunc"<<count<<")("<< manage_types(*x,false, true)  <<" x)," << endl;
	}
      }	
    } 
    myfile << "int startvararg, ...){" << endl;
   
    
    myfile << "  printf(\"INTO: "<<function_name<<" \\n\" );" << endl;

    string namestruct = "Mock_"+ function_name;
    myfile << "  struct "<< namestruct<< " *obj = new "<< namestruct <<"();" << endl;
    myfile << "  list_"<< function_name << ".push_front(obj);"<< endl;     
   
    for (count = 0; ( !list_type_args.empty() ) && count < list_type_args.size()-1 ;  count++){ //size()-1 because we need always to skip the last argument. size()-2 because here I dont consider the "int startvararg" parameter.
     
 //myfile << " printf(\"param"<<count<<": %g, switch"<<count<<": %g  \\n \", param"<<count<< " , switch"<<count<<" );"<< endl;
     
      myfile << "  obj->switch" << count << "= switch"<< count<<";"<< endl; 
      myfile << "  obj->param" << count << "= param"<< count<<";"<< endl;
      myfile << "  obj->userfunc" << count << "= userfunc"<< count<<";"<< endl;
      myfile << endl;
    }

    myfile << endl;
    myfile << "//Add Times Expectations" << endl;
    myfile << "  if(startvararg == NO_MORE_PARAMETERS ){ // use the default setting" << endl;
    myfile << "   printf(\"no more parameters  \\n \");" << endl;
    myfile << "   return;" << endl;
    myfile << "  }" << endl;
    myfile << "  va_list ap; " << endl;
    myfile << "  va_start(ap, startvararg);" << endl;
    myfile << "  int time_value;" << endl;
    //  myfile << "  printf(\"startvararg: %d\\n \", startvararg);" << endl;
    myfile << "  if(startvararg == TIMES || startvararg == TIMES_AT_LEAST || startvararg == TIMES_AT_MOST){" << endl;
    myfile << "    time_value = va_arg(ap,int);" << endl;

    myfile << "    struct Expectation* exp = new Expectation(); "<< endl;  
    myfile << "    exp->type_expectation = startvararg; " << endl;
    myfile << "    int* p = (int*)malloc(sizeof(int)); " << endl;
    myfile << "    *p = time_value;" << endl;
    myfile << "    exp->returnvalue = (void*)p;" << endl;
    myfile << "    obj->list_expectations.push_back(exp);" << endl;
    myfile << "  }else{ " << endl; 
    myfile << "    time_value = va_arg(ap,int); // to jump the \"time_value parameter\" used from the TIMES macro " << endl;
    myfile << "  }" << endl;
    myfile << "  //Manage WILLONCE and REPEADETLY" << endl;
    
    if(return_type.compare("void")!=0 ){  // you do not need to manage return type for function that return void.  


      myfile << "  int type_expectation; // the type of the variable is as the type return function" << endl;
      myfile << "  " << return_type << " return_value;" << endl;
      myfile << "  for(type_expectation = va_arg(ap, int); type_expectation != NO_MORE_PARAMETERS; type_expectation = va_arg(ap,int) ){" << endl;
      myfile << "    struct Expectation* exp = new Expectation(); "<< endl;
      myfile << "    return_value = va_arg(ap, " << return_type << ");// the return type is always as the return type of the orginal function. We are treatting just WILLONCE and REPEADETLY " << endl;
      myfile << "    "<< return_type << "* p = ("<<return_type<<"*)malloc(sizeof("<<return_type<<"));" << endl;
      myfile << "    *p = return_value;" << endl;
      myfile << "    exp->type_expectation = type_expectation; " << endl;
      myfile << "    exp->returnvalue = (void*)p; " << endl; 
      //  myfile << "    printf(\"type_expectation: %d  \\n\", type_expectation);  " << endl;
      //myfile << "    printf(\"return value: %d \\n\",return_value);//occhio che non e sempre di tipo intero  " << endl;   
      myfile << "    obj->list_expectations.push_back(exp);" << endl;
      myfile << "  }" << endl;
    }
    myfile << "  va_end(ap);" << endl;
      

    myfile << "}" << endl;
    myfile << endl;  
  }
 

}




void write_structs(){

 for (list<tree_node*>::iterator it = list_functions_to_mock.begin(); it != list_functions_to_mock.end(); ++it){
   list<string> list_type_args; 
   list<tree_node*> list_node_args;

   cout << endl;
   string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it)); 
   tree_node *tn =  reinterpret_cast<tree_node*>(*it);
   tree get_decl_result = DECL_RESULT(tn);   
   // string return_type =  manage_types(TREE_TYPE( TREE_TYPE(*it) ), false);
   //myfile << return_type << " " << function_name << "("; 
  
   cout << "Writing the struct for " << function_name << endl;
   myfile << "struct Mock_" << function_name << "{"<< endl;
   
   //  debug_tree(TREE_TYPE(*it));
   tree node = TYPE_ARG_TYPES( TREE_TYPE(*it) );//get function's arguments
   if(node != NULL){ //if node is null the function does NOT have arguments
     do{
       list_type_args.push_back( manage_types(TREE_VALUE(node), false, true ) );//for each arguments: get the type argument and put it into the list_type_args
       list_node_args.push_back( TREE_VALUE(node) );
     }while(node = TREE_CHAIN(node) );
   
     //write on temporary_file.c the type arguments
     int count =0;
     //it is necessary jump the last element that its always (a void type) not necessary
     
     for (list<tree_node*>::iterator x = list_node_args.begin();  count < list_node_args.size()-1 ; ++x, count++){
       
       if(TREE_CODE(*x) == POINTER_TYPE){
         if(TREE_CODE(TREE_TYPE(*x)) == FUNCTION_TYPE){
	   myfile << "  int switch"<< count << "; " << manage_function_type(TREE_TYPE(*x),count)  << ";" << " int (*userfunc" << count << ")("<< manage_function_type(TREE_TYPE(*x), count) << ");"  << endl;
	 }
       }else if(TREE_CODE(*x) == RECORD_TYPE){
	 myfile << "  int switch"<< count << "; " << manage_types(*x,false, true) << "* param" << count << ";" << " int (*userfunc" << count << ")("<< manage_types(*x, false, true) << " x);"  << endl;
       }else{
	 myfile << "  int switch"<< count << ";" <<"  " << manage_types(*x,false, true) << " param" << count << ";" << " int (*userfunc" << count << ")("<< manage_types(*x,false, true) << " x);"  << endl;
       }
     }
   }
   
   myfile << "  list<Expectation*> list_expectations;" << endl;
   myfile << "};" << endl;
   myfile << "list<Mock_"<< function_name << "*> list_" << function_name << ";"<< endl; 
   // string up_function_name = function_name;
   //transform(up_function_name.begin(), up_function_name.end(), up_function_name.begin(), toupper);
   //myfile << "#define EXPECT_" << function_name << "(...) expect_" << function_name << "( __VA_ARGS__, NO_MORE_PARAMETERS)" << endl; 
   myfile << endl;
 }

   
}
 

void write_header(){
  
  headerfile.open("temporary_file.h");
  headerfile << "#ifndef TEMPORARYFILE_H"<< endl;
  headerfile << "#define TEMPORARYFILE_H" << endl;
  headerfile << "#include \"user_headers.h\"" << endl;
  //headerfile << "#include \"temporary_user_structs.h\""<< endl;
  for (list<tree_node*>::iterator it = list_functions_to_mock.begin(); it != list_functions_to_mock.end(); ++it){
    list<string> list_type_args; 
    list<tree_node*> list_node_args;
    
    cout << endl;
    string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it));
    cout << "Writing header for the function: " << function_name << endl;
    tree_node *tn =  reinterpret_cast<tree_node*>(*it);
    tree get_decl_result = DECL_RESULT(tn);      
  
    headerfile << "#define EXPECT_" << function_name << "(...) expect_" << function_name << "( __VA_ARGS__, NO_MORE_PARAMETERS)" << endl; 
    headerfile << "void expect_" << function_name << "("; 
    tree node = TYPE_ARG_TYPES( TREE_TYPE(*it) );//get function's arguments
    if(node!=NULL){
      do{
	list_type_args.push_back( manage_types(TREE_VALUE(node),false, true ));//for each arguments: get the type argument and put it into the list_type_args
	list_node_args.push_back(TREE_VALUE(node));
      }
      while(node = TREE_CHAIN(node) );
      
      //write on temporary_file.c the type arguments
      int count =0;
      //it is necessary jump the last element that its always (a void type) not necessary
      for (list<tree_node*>::iterator x = list_node_args.begin(); count < list_node_args.size()-1 ; ++x, count++){
	
	if(TREE_CODE(*x) == POINTER_TYPE){
	  if(TREE_CODE(TREE_TYPE(*x)) == FUNCTION_TYPE){
	    headerfile << "int switch" << count << ", " << manage_function_type(TREE_TYPE(*x), count) << ", int (*userfunc"<<count<<")("<< manage_function_type(TREE_TYPE(*x),count) <<")";
	  }
	}else if( (TREE_CODE(*x) == RECORD_TYPE) ||(TREE_CODE(*x) == ENUMERAL_TYPE)){ 
	  headerfile << "int switch" << count << ", " << manage_types(*x, false, true) << "* param" << count << ", int (*userfunc"<<count<<")(" << manage_types(*x, false, true) <<" x)";
	}else{
	  headerfile << "int switch" << count << ",  " << manage_types(*x, false, true) << " param" << count << ", int (*userfunc"<<count<<")("<< manage_types(*x,false, true)  <<" x)";
	}
	
	if(count != list_node_args.size()-2){
	  headerfile << ", ";
	}
      }
      headerfile << ", int startvararg, ...); " << endl;
    }else{
      headerfile << "int startvararg, ...); " << endl;
    }
  }

    headerfile << endl;  
    headerfile << "#define TEST_FINISHED testCompleted()" << endl;
    headerfile << "void testCompleted();" << endl;
    headerfile << "#endif" << endl;
    headerfile.close();
}



void write_check_testCompleted_function(){
  myfile << endl;
  
  myfile << "//When the test is finished we have to see if all the expectations have been respected or not." << endl;
  myfile << "extern \"C\" " << endl;
  myfile << "void testCompleted(){"<<endl;
  myfile << "  int expectation_number = 1 ;"<< endl;  
  myfile << endl;
  for (list<tree_node*>::iterator it = list_functions_to_mock.begin(); it != list_functions_to_mock.end(); ++it){
    string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it)); 
    myfile << "  for ( list<Mock_" << function_name << "*>::iterator it = list_" << function_name << ".begin(); it!= list_" << function_name <<".end() ;expectation_number++, ++it){" << endl;

    
    myfile << "    struct Mock_"<< function_name <<"* obj_"<<function_name <<" = *it;"<< endl;
    myfile << "    std::stringstream ss ;" << endl;
    myfile << "    ss << " << "\"" << function_name << "\"" <<" << \" from Expectation number: \"<< expectation_number; " << endl; 
    myfile << "    check_expectations_test_finished(obj_"<< function_name <<"->list_expectations , ss.str() );" << endl;
    myfile << "  }" << endl;
    myfile << endl;
    myfile <<"   expectation_number = 1;" << endl;
  }    
  myfile << "}"<< endl;
}




void write_function_on_file(){

  cout << "-- Write function calls to mock into the temporary file: temporary_file.c "<< endl; 
  myfile.open("temporary_file.cpp");
  myfile << "#include \"helper.h\" "<< endl;
  myfile << "extern \"C\" {"<< endl;
  myfile << "  #include <stdarg.h>" << endl;
  myfile << "#include <string.h>//strcmp()" << endl;
  // myfile << "#include \"temporary_file.h\""<< endl;
  myfile << "#include <stdio.h>"<< endl;
  myfile << "#include <stdlib.h>"<< endl;
  //myfile << "#include \"helper.h\""<< endl;
  myfile << "}"<< endl;
  myfile << "#include <sstream>" << endl;
  myfile << "#include <list>"<< endl;
  myfile << "#include <iostream>" << endl;
  //  myfile << "#include \"temporary_user_structs.h\"" << endl;
  myfile << "#include \"user_headers.h\"" << endl;
  myfile << endl;
  
  myfile << "using namespace std;" << endl;
  //============
 
  cout << "debug 1 " << endl;
  write_structs();
  myfile << endl;
  cout << "debug 2 " << endl;
  write_expectation_functions();
  cout << "debug 3 " << endl;

  write_external_functions();
  cout << "debug 4" << endl;
  write_check_testCompleted_function();
  cout << "debug 5" << endl; 
  myfile.close();
} 

//Function to re-built all the user data definition. Not used
void write_user_structs(){
  cout << "+++ Into write_user_structs +++" << endl;
  userStructDefined_file.open("temporary_user_structs.h");
  list<string> list_struct;

  for (list<tree_node*>::iterator it = list_functions_to_mock.begin(); it != list_functions_to_mock.end(); ++it){
    
    string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it));
    cout << "Checking if there are user-structs as parameters into the function: " << function_name<< endl;
    tree_node *tn =  reinterpret_cast<tree_node*>(*it);
    tree get_decl_result = DECL_RESULT(tn);      
  
    tree node = TYPE_ARG_TYPES( TREE_TYPE(*it) );//get function's arguments
    if(node!=NULL){
      do{
	if(( TREE_CODE(  TREE_VALUE(node)) == ENUMERAL_TYPE)){
	   bool isPresent = false;
	  for(list<string>::iterator user_enum_it = list_struct.begin(); user_enum_it != list_struct.end() ; user_enum_it++){ 
	   
	    if( (*user_enum_it).compare( manage_enumeral_type(TREE_VALUE(node),true )  ) == 0 ){
	      isPresent = true;
	    }

	  }
	  if(!isPresent){//write the struct in the user structs into the file "temporary_user_structs.h"
	    list_struct.push_back(manage_enumeral_type(TREE_VALUE(node), true) );
	    userStructDefined_file  <<  manage_enumeral_type(TREE_VALUE(node), false);
	  }
	}
	
	if ( (TREE_CODE(  TREE_VALUE(node)) == RECORD_TYPE) ){ //check if the parameter is a user struct
	  bool isPresent = false;
	  for(list<string>::iterator userstruct_it = list_struct.begin(); userstruct_it != list_struct.end() ; userstruct_it++){ //check if I have already written the same struct
	    
	    if( (*userstruct_it).compare( manage_record_type(TREE_VALUE(node),true )  ) == 0 ){
	      isPresent = true;
	    }

	  }
	  if(!isPresent){//write the struct in the user structs into the file "temporary_user_structs.h"
	    list_struct.push_back(manage_record_type(TREE_VALUE(node), true) );
	    userStructDefined_file  << "typedef " << manage_record_type(TREE_VALUE(node), false);
	  }	  
	}    
      }
      while(node = TREE_CHAIN(node) );
    }  
}
  userStructDefined_file.close();

}

void find_functions_to_mock(){
  cout << "##### find functions to mock ###### " << endl;
  for (list<tree_node*>::iterator it = list_functions_with_only_declaration.begin(); it != list_functions_with_only_declaration.end(); ++it){
    tree id = DECL_NAME(*it);
    string name_function_with_onyl_declaration (IDENTIFIER_POINTER (id));
    bool isPresent = false;
    cout << "function name with: " << name_function_with_onyl_declaration << endl;
    for (list<tree_node*>::iterator it2 = list_functions_with_body.begin(); (!isPresent) && it2 != list_functions_with_body.end(); ++it2){
      tree id2 = DECL_NAME(*it2);
      string name_function_with_body (IDENTIFIER_POINTER (id2));
      cout << "function name: " << name_function_with_onyl_declaration << endl;
      if(name_function_with_body.compare(name_function_with_onyl_declaration) == 0){
	isPresent = true;
      }
    }
    if(!isPresent){
      list_functions_to_mock.push_back(*it);
    }    
  }
}

void cb_plugin_finish(void *gcc_data, void *user_data){
  cout << "****** EVENT PLUGIN_FINISH *******" << endl;
  find_functions_to_mock();
  cout<<"Print Functions to mock" <<endl; 
  print_list(list_functions_to_mock);
  cout <<endl;
  //write_user_structs(); 
  write_function_on_file();
  write_header();
  cout << " *** END PLUGIN **** " << endl;
}




int plugin_init (plugin_name_args* info,
             plugin_gcc_version*)
{
  int r (0);

  cerr << "starting " << info->base_name << endl;
  //
  // Parse options if any.
  //

  // Disable assembly output.
  //
  // asm_file_name = HOST_BIT_BUCKET;
  
  // Register callbacks. 
  register_callback(info->base_name,
		    PLUGIN_FINISH_DECL,
		    &cb_finish_decl,
		    0);

  register_callback(info->base_name,
		    PLUGIN_PRE_GENERICIZE,
		    &cb_plugin_pre_generize,
		    0);

  register_callback(info->base_name,
		    PLUGIN_FINISH,
		    &cb_plugin_finish,
		    0);
  
  return r;
}
