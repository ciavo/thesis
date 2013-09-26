// GCC header includes to get the parse tree declarations. The order
// is important and doesn't follow any kind of logic.
//

#include <stdlib.h>
#include <gmp.h>

#include <cstdlib> // Include before GCC poison some declarations.

extern "C"
{
#include "gcc-plugin.h"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-iterator.h"
#include "intl.h"

#include "tm.h"

#include "diagnostic.h"
#include "c-common.h"
#include "c-pragma.h"
#include "cp/cp-tree.h"
}
#include <set>
#include <string>
#include <iostream>

//my inclusions
#include <list>
#include <iostream>
#include <fstream>

using namespace std;

int plugin_is_GPL_compatible;
 

extern "C" void myprint_declaration(tree decl);
extern "C" string manage_types(tree nodetype);
extern "C" void navigate_statements(tree statement_list);

list<tree_node*> list_functions_call_without_body;
list<tree_node*> list_functions;
list<tree_node*> list_functions_tomock;

ofstream myfile;



extern "C" void
gate_callback (void* tree, void*)
{
  // If there were errors during compilation,
  // let GCC handle the exit.
  //
  cout << "Into gate callback "<< endl;
  if (errorcount || sorrycount)
    return;

  tree_node *tn = reinterpret_cast<tree_node*>(tree);
  //myprint_declaration(tn); 
  //  debug_tree(tn);

  int r (0);

  //
  // Process AST. Issue diagnostics and set r
  // to 1 in case of an error.
  //
  cerr << "processing " << main_input_filename << endl;
  //traverse (global_namespace);

  exit (r);
}

  
extern "C" void
myprint_declaration(tree decl) //conflit error if you name the function: print_declaration
{
  int treecode = TREE_CODE(decl);
  tree id = DECL_NAME(decl);
  // const char* name (id ? IDENTIFIER_POINTER (id): "<unnamed>");
  
  cout << "*** ~~~ "  << tree_code_name[treecode]  << " "/* << name <<*/ " at "
       <<  DECL_SOURCE_FILE(decl) << ":"
       <<  DECL_SOURCE_LINE(decl) << "~~~ ***" << endl;
}

extern "C" void
manage_function_decl(tree func_decl_node ){
  cout << "---- MANAGE FUNCTION DECLARATION ---- " << endl;
 
  list_functions_call_without_body.push_back(func_decl_node);
  
  //  cout << "debug 1.1" << endl;
  myprint_declaration(func_decl_node);
  /*
  // debug_tree(func_decl_node); 
  cout << "---GET THE RETURN TYPE---"<< endl;
  // tree result_fndecl = TREE_TYPE(tn);
  tree_node *tn =  reinterpret_cast<tree_node*>(func_decl_node);
  tree get_decl_result = DECL_RESULT(tn);
  cout << "debug 1.1.1 --- get the name:" << IDENTIFIER_POINTER(DECL_NAME(tn)) << endl;

  cout << "debug 1.2. ----  1 means that the function is external:  " << DECL_EXTERNAL(tn) <<  endl;
  // debug_tree( TREE_TYPE(tn));
  cout << "debug 1.3 -- debug tree of TREE_TYPE( TREE_TYPE(tn))" << endl;
  //debug_tree(TREE_TYPE( TREE_TYPE(tn) ));
  manage_types(TREE_TYPE( TREE_TYPE(tn) )); // gestisci il valore di ritorno!
  cout << "debug 1.4-- take arguments " << endl;
  tree node = TYPE_ARG_TYPES( TREE_TYPE(tn) );
  //debug_tree(TYPE_ARG_TYPES( TREE_TYPE(tn) ));
  //cout << "Tree_code (get_decl_result): " << TREE_CODE(get_decl_result) << endl;
  do{
    manage_types(TREE_VALUE(node));
  }
  while(node = TREE_CHAIN(node) );
  */

  cout << "---- END ---  MANAGE FUNCTION DECLARATION ---- " << endl;
}


//to manage function calls
extern "C" void
manage_call_expr(tree statement, int i){
    cout << "----  Manage CALL_EXPR  ----" << endl;
    // myprint_declaration(statement);x3
    cout << "---- Fn of stattement number: "<< i << "****" << endl;
    // debug_tree(CALL_EXPR_FN(statement) );
    cout << "---- End Fn "<< i << "****" << endl;
    
    tree fndecl = TREE_OPERAND(CALL_EXPR_FN(statement), 0);//  In this link they do the same thing : lwn.net/Articles/457543/

    tree_node *fndecl_node = reinterpret_cast<tree_node*>(fndecl); //fndecl_node is a FUNCION_DECL type node
    // myprint_declaration(fndecl_node);
    // cout << "debug 1.0"<< endl;
    //debug_tree(fndecl_node);
    manage_function_decl(fndecl);
    cout << "debug 2.0"<< endl;
 // debug_tree(fndecl);
  
    cout << "---- END into fndecl" << endl;
    cout << "---- END Manage CALL_EXPR  ----" << endl;
}

extern "C" void
manage_cond_expr(tree tree_if, int i )
{
   cout << "----  Manage COND_EXPR  ----" << endl;

   cout << "----  THEN Part   ----" << endl; 

   tree_node *mytree = reinterpret_cast<tree_node*>(tree_if);  
   tree thenside_node = COND_EXPR_THEN(mytree);
   // debug_tree(thenside_node);
   navigate_statements(thenside_node);
   cout << "----  ELSE Part   ----" << endl; 
   navigate_statements(COND_EXPR_ELSE(mytree));
   cout << "---- END Manage COND_EXPR  ----" << endl;

}

extern "C" void
manage_goto_expr(tree my_tree, int i){
   cout << "----  Manage GOTO_EXPR  ----" << endl;
   tree gotonode = GOTO_DESTINATION(my_tree);
   debug_tree(gotonode);
   cout << "----  Manage GOTO_EXPR  ----" << endl;
}

extern "C" void
manage_decl_expr(tree my_tree,int i){
  /* cout << " --- Manage DECL_EXPR --- " << endl;
  debug_tree(my_tree);
   cout << " ****-- debug 2.1 --**** "<< endl;
  debug_tree( DECL_EXPR_DECL(my_tree));
  
   cout << " ****-- debug 2.2 --**** "<< endl;
   debug_tree( DECL_CONTEXT(my_tree) );
  // debug_tree( COMPOUND_LITERAL_EXPR_DECL_EXPR(my_tree) ); 
  cout << " --- END Manage DECL_EXPR --- " << endl; */
}

extern "C" void
manage_tree_node(tree my_tree, int i){
  
  tree_node *my_treenode = reinterpret_cast<tree_node*>(my_tree);
   
  switch(TREE_CODE(my_treenode)){
    case CALL_EXPR:
      manage_call_expr(my_tree,i); //manage function calls
      break;

    case COND_EXPR: 
      manage_cond_expr(my_tree,i);
      break;

    case GOTO_EXPR:
      manage_goto_expr(my_tree,i);
      break;

    case DECL_EXPR:
      manage_decl_expr(my_tree,i);
      break;

    default:
      break;
  }
}

extern "C" void
navigate_statements(tree statement_list){

  cout << "************** Statements navigation ******************" << endl;
  int i = 1;
  tree_stmt_iterator stm_iterator = tsi_start(statement_list );

  while(!tsi_end_p(stm_iterator)){
    cout << "**** statement "<< i << "****" << endl;

    tree statement = tsi_stmt(stm_iterator);
    debug_tree(statement);
    tree_node *statement_node = reinterpret_cast<tree_node*>(statement);

    manage_tree_node(statement_node, i);
                   
    tsi_next(&stm_iterator);
    i++;
  }
  
  cout << "************** END statements navigation ******************" << endl;
}

static int count_calls = 0;


extern "C" void
navigate_tree(tree_node *tree){
  //if the main function contains only one instruction => there is no a list of statements
  
  if(TREE_CODE(tree) == STATEMENT_LIST ){
    navigate_statements( tree );
  }else{
    manage_tree_node(tree, 0);
  }

}

extern "C" string 
manage_types(tree nodetype)
{
  string result;
  cout << "Node Type: " <<  tree_code_name[TREE_CODE(nodetype)] << endl;
  switch(TREE_CODE(nodetype)){
   case INTEGER_TYPE: 
     if( TYPE_STRING_FLAG(nodetype) == 1 ){
       result = "char";
       cout <<" Type: char " <<  endl;
     }else{
       cout <<" TYPE_PRECISION:  " << TYPE_PRECISION(nodetype)  << endl;
       switch( TYPE_PRECISION(nodetype)){
        case 16 :
	  result = "short";
	  cout << "type: short"<< endl;
          break; 
        case 32 :
	  result = "int";
	  cout << "type: int"<< endl;
          break; 
        case 64 :
	  result = "long";
	  cout << "type: long"<< endl;
          break; 
       }
     }
     break;

  case REAL_TYPE:
    cout <<" TYPE_PRECISION:  " << TYPE_PRECISION(nodetype)  << endl;
    switch( TYPE_PRECISION(nodetype)){
      case 32 :
	result = "float";
	cout << "type: float"<< endl;
	break; 
      case 64 :
	result = "double";
	cout << "type: double"<< endl;
	break; 
      case 80:
	 result = "long double";
	cout << "type: long double" << endl;
    }
    break;

  case VOID_TYPE:
    result="void";
    break;
  case COMPLEX_TYPE:
    break;
  case ENUMERAL_TYPE:
    break;
  case BOOLEAN_TYPE:
    break;
  case POINTER_TYPE:
    break;
  default:
     break;
  }
  cout << endl;
  cout << "DENTRO GESTIONE TIPI. RISULTATO= "<< result << endl;
    cout << endl;
  return result;
}

extern "C" void
cb_plugin_pre_generize (void *maintree, void*)
{
  count_calls ++ ;
  
  cout << "************** In my callback function. Call number: " << count_calls << "*******************"<< endl;
 
  tree_node *tn = reinterpret_cast<tree_node*>(maintree);
  myprint_declaration(tn); 
  
  list_functions.push_back(tn);

  tree get_decl_result = DECL_RESULT(tn);
  cout << " --- Function name: " << IDENTIFIER_POINTER(DECL_NAME(tn)) << " ---" << endl;
  
  // debug_tree(tn);
  // GET the return type, the arguments and the type arguments of the main function
  
  cout << "---GET THE RETURN TYPE---"<< endl; //here it works because DECL_EXTERNAL(tn) != 1

  // tree get_decl_result = DECL_RESULT(tn);

  cout << "Tree_code (get_decl_result): " << TREE_CODE(get_decl_result) << endl;

  // debug_tree(get_decl_result);
  cout << "-------------------get the type?" << endl;
  tree get_type_decl_result = TREE_TYPE(get_decl_result);
  // debug_tree(get_type_decl_result);

  string return_type =  manage_types(get_type_decl_result);
  
  cout << "--- END -- GET THE RETURN TYPE ---"<< endl;

  
  cout << "-------get the FUNCTION arguments -----------" << endl;
  tree get_arguments_node = DECL_ARGUMENTS(tn);
  debug_tree(get_arguments_node);
  do{ 
    cout << "------ get the argument type ---------" << endl;
    if(get_arguments_node == NULL_TREE){
      cout << "NO ARGUMENTS " << endl;
    }else{
      // tree get_type_arg = TREE_TYPE(get_arguments_node);
      tree get_type_arg = DECL_ARG_TYPE(get_arguments_node);
      cout << "tree code Return type argument of the main function: " <<  TREE_CODE(get_type_arg) << endl;
      cout << "Return type argument of the main function: " <<  tree_code_name[TREE_CODE(get_type_arg)] << endl;
      //debug_tree(get_type_arg);
      string argtype =  manage_types(get_type_arg);
    } 
  }while(get_arguments_node = TREE_CHAIN(get_arguments_node));


  

  //debug_tree(tn);
  tree_node* main_body = BIND_EXPR_BODY( DECL_SAVED_TREE(tn) ); 
  cout << "************** Into the body function ******************" << endl;
      

  navigate_tree(main_body);
  cout << "************** out body function ******************" << endl;

  cout << "**************** Ends my callback functin **********************" << endl;
}



//=======================================================================================================



extern "C"
void print_declarationfunction(tree_node * node){
  const char* name = IDENTIFIER_POINTER( DECL_NAME(node) );
  
  cout << "*** ~~~ "  << tree_code_name[ TREE_CODE(node)]  << " " << name << " at "
       <<  DECL_SOURCE_FILE(node)<< ":"
       <<  DECL_SOURCE_LINE(node)<< "~~~ ***" << endl;

}
/*
extern "C"
void print_list(list<tree_node*> list){
 
  for (list<tree_node*>::iterator it = list.begin(); it != list.end(); ++it){
    print_declarationfunction(*it);
  }

  }*/


extern "C"
void write_function_on_file(){
  cout << "-- Write function calls to mock into the temporary file: temporary_file.c "<< endl; 
  myfile.open("temporary_file.c");
 
  //============
  for (list<tree_node*>::iterator it = list_functions_tomock.begin(); it != list_functions_tomock.end(); ++it){
    list<string> list_type_args;
    cout << endl;
    print_declarationfunction(*it);
    cout << endl;
    debug_tree(*it);  
    cout << endl;
    string function_name =  IDENTIFIER_POINTER(DECL_NAME(*it));
    cout << "Function name:" << function_name<< endl;

    cout << "---GET THE RETURN TYPE---"<< endl;
    tree_node *tn =  reinterpret_cast<tree_node*>(*it);
    tree get_decl_result = DECL_RESULT(tn);
   

    cout << "debug 1.2. ----  1 means that the function is external:  " << DECL_EXTERNAL(*it) <<  endl;
   
  
    string return_type =  manage_types(TREE_TYPE( TREE_TYPE(*it) ));
    myfile << return_type << " " << function_name << "("; 
  
    cout << "return type: "<< return_type << endl;
    cout << endl;
  
    cout << "take arguments " << endl;
    tree node = TYPE_ARG_TYPES( TREE_TYPE(*it) );

    do{
      list_type_args.push_back( manage_types(TREE_VALUE(node)));//for each arguments: get the type argument and put it into the list_type_args
    }
    while(node = TREE_CHAIN(node) );

    cout << endl;
    cout << "Print type arguments"<<endl;
    //print type arguments
    int count =0;

    //its necessary jump the last element that its always (a void type) not necessary
    for (list<string>::iterator it = list_type_args.begin();count < list_type_args.size()-1 ; ++it, count++){
      
      myfile << *it << " param" << count;
      if(count != list_type_args.size()-2){
	myfile << ", ";
      }
    }
    myfile << "){" << endl;
    myfile << "  printf( \" It works!!! \"  ); " << endl; 
    myfile << "}" << endl;
    myfile << endl;  
  }
    
  myfile.close();
}



extern "C" 
void cb_plugin_finish(void *gcc_data, void *user_data){
  cout << "****** EVENT PLUGIN_FINISH *******" << endl;
  cout << "Read list_functions:"<< endl;
  cout << endl;


  //print_list(list_functions);


  for (list<tree_node*>::iterator it = list_functions.begin(); it != list_functions.end(); ++it){
    //  tree_node *node = reinterpret_cast<tree_node*>(it);
    print_declarationfunction(*it);
  }
  cout << endl;

  cout << "Read list_functions_call_without_body: "<< endl;
  cout << endl;

  for (list<tree_node*>::iterator it = list_functions_call_without_body.begin(); it != list_functions_call_without_body.end(); ++it){
    //  tree_node *node = reinterpret_cast<tree_node*>(it);
    print_declarationfunction(*it);
  }
  cout << endl;
 
  
  //Find the functions to mock
 
  for (list<tree_node*>::iterator it =list_functions_call_without_body.begin(); it !=list_functions_call_without_body.end(); ++it){      
    bool isPresent = false;
    string source_file = DECL_SOURCE_FILE(*it);
    int number_line =  DECL_SOURCE_LINE(*it);
    size_t found = source_file.find("/usr/include"); 
    if(found!=string::npos ){
      isPresent = true;     
    }
    for (list<tree_node*>::iterator it2 = list_functions.begin(); it2 != list_functions.end() && !isPresent  ; ++it2){      
      string source_file2 = DECL_SOURCE_FILE(*it2); 
      int number_line2 =  DECL_SOURCE_LINE(*it2);
      if( (source_file.compare(source_file2) == 0) && (number_line == number_line2) ){
	isPresent = true;
      }      
    }
    for (list<tree_node*>::iterator it3 = list_functions_tomock.begin(); it3 != list_functions_tomock.end() && !isPresent  ; ++it3){ 
      string source_file3 = DECL_SOURCE_FILE(*it3); 
      int number_line3 =  DECL_SOURCE_LINE(*it3);
      if( (source_file.compare(source_file3) == 0) && (number_line == number_line3) ){
	isPresent = true;
      } 
    }
    if(!isPresent){
      list_functions_tomock.push_back(*it);
    }
  }

  cout<<"Functions to mock" <<endl;
  for (list<tree_node*>::iterator it = list_functions_tomock.begin(); it != list_functions_tomock.end(); ++it){
    print_declarationfunction(*it);
  }
  //  print_list(list_functions_tomock);
  cout <<endl;
  write_function_on_file();
}

extern "C" int
plugin_init (plugin_name_args* info,
             plugin_gcc_version*)
{
  int r (0);

  cerr << "starting " << info->base_name << endl;
  //
  // Parse options if any.
  //

  // Disable assembly output.
  //
  asm_file_name = HOST_BIT_BUCKET;
  
  // Register callbacks.
  //
  
  register_callback(info->base_name,
		    PLUGIN_PRE_GENERICIZE,
		    &cb_plugin_pre_generize,
		    0);

  register_callback(info->base_name,
		    PLUGIN_FINISH,
		    &cb_plugin_finish,
		    0);
  /*
   register_callback (info->base_name,
                        PLUGIN_OVERRIDE_GATE,
                        &gate_callback,
                      0);
  */ 
  return r;
}
