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

using namespace std;

int plugin_is_GPL_compatible;

extern "C" void
gate_callback (void*, void*)
{
  // If there were errors during compilation,
  // let GCC handle the exit.
  //
  if (errorcount || sorrycount)
    return;

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
manage_call_expr(tree statement, int i){
    cout << "----  Manage CALL_EXPR  ----" << endl;
    cout << "---- Fn of stattement number: "<< i << "****" << endl;
    debug_tree(CALL_EXPR_FN(statement) );
    cout << "---- End Fn "<< i << "****" << endl;
    
    tree fndecl = TREE_OPERAND(CALL_EXPR_FN(statement), 0);//  In this link they do the same thing : lwn.net/Articles/457543/

    tree_node *fndecl_node = reinterpret_cast<tree_node*>(fndecl);
    
    cout << "---- Function name: " <<   IDENTIFIER_POINTER(DECL_NAME(fndecl_node)) << endl;
    
    debug_tree(fndecl); 
    
    cout << "---- END Manage CALL_EXPR  ----" << endl;
}

extern "C" void
manage_tree_node(tree my_tree, int i){
  
  tree_node *my_treenode = reinterpret_cast<tree_node*>(my_tree);
  
  switch(TREE_CODE(my_treenode)){
      case CALL_EXPR:
           manage_call_expr(my_tree,i);
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

extern "C" void
cb (void *tree, void*)
{
  count_calls ++ ;
  
  cout << "************** In my callback function. Call number: " << count_calls << "*******************"<< endl;
  tree_node *tn = reinterpret_cast<tree_node*>(tree);
  debug_tree(tn);
  
  cout << "************** Into the body function ******************" << endl;
  debug_tree( BIND_EXPR_BODY( DECL_SAVED_TREE(tn) ) );
  
  tree_node *main_body = BIND_EXPR_BODY( DECL_SAVED_TREE(tn) );

  navigate_tree(main_body);

  cout << "************** out body function ******************" << endl;

  cout << "**************** Ends my callback functin **********************" << endl;
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
		    &cb,
		    0);

  // register_callback (info->base_name,
  //                    PLUGIN_OVERRIDE_GATE,
  //                    &gate_callback,
  //                    0);
  return r;
}
