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

struct decl_comparator
{
  bool
  operator() (tree x, tree y) const
  {
    location_t xl (DECL_SOURCE_LOCATION (x));
    location_t yl (DECL_SOURCE_LOCATION (y));

    return xl < yl;
  }
};

typedef std::multiset<tree, decl_comparator> decl_set;

void
collect (tree ns, decl_set& set)
{
  tree decl;
  cp_binding_level* level (NAMESPACE_LEVEL (ns));

  // Collect declarations.
  //
  for (decl = level->names; decl != 0; decl = TREE_CHAIN (decl))
  {
    if (DECL_IS_BUILTIN (decl))
      continue;

    set.insert (decl);
  }

  // Traverse namespaces.
  //
  for(decl = level->namespaces; decl != 0; decl = TREE_CHAIN (decl))
  {
    if (DECL_IS_BUILTIN (decl))
      continue;

    collect (decl, set);
  }
}


void
print_decl (tree decl);

enum access_spec
{
  public_, protected_, private_
};

const char* access_spec_str[] =
{
  "public", "protected", "private"
};


void
print_decl (tree decl)
{
  tree type (TREE_TYPE (decl));
  int dc (TREE_CODE (decl));
  int tc;

  if (type)
  {
    tc = TREE_CODE (type);

    if (dc == TYPE_DECL && tc == RECORD_TYPE)
    {
      // If DECL_ARTIFICIAL is true this is a class
      // declaration. Otherwise this is a typedef.
      //
      if (DECL_ARTIFICIAL (decl))
      {
	//        print_class (type);
        return;
      }
    }
  }

  tree id (DECL_NAME (decl));
  const char* name (id
                    ? IDENTIFIER_POINTER (id)
                    : "<unnamed>");

  // cerr << tree_code_name[dc] << " "
  //      << decl_scope (decl) << "::" << name;

  if (type)
    cerr << " type " << tree_code_name[tc];

  cerr << " at " << DECL_SOURCE_FILE (decl)
       << ":" << DECL_SOURCE_LINE (decl) << endl;
}

void
traverse (tree ns)
{
  decl_set set;
  collect (ns, set);

  for (decl_set::iterator i (set.begin ()), e (set.end ());
       i != e; ++i)
  {
    print_decl (*i);
  }
}

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
cb (void *tree, void*)
{
  cout << "In my callback function" << endl;
  tree_node *tn = reinterpret_cast<tree_node*>(tree);
  debug_tree(tn);
  cout << "Ends my callback functin" << endl;
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
