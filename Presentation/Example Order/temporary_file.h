#ifndef TEMPORARYFILE_H
#define TEMPORARYFILE_H
#include "user_headers.h"
#define EXPECT_warehouse_has_inventory(...) expect_warehouse_has_inventory( __VA_ARGS__, NO_MORE_PARAMETERS)
void expect_warehouse_has_inventory(int switch0,  char* param0, int (*userfunc0)(char* x), int switch1,  int param1, int (*userfunc1)(int x), int startvararg, ...); 
#define EXPECT_warehouse_remove(...) expect_warehouse_remove( __VA_ARGS__, NO_MORE_PARAMETERS)
void expect_warehouse_remove(int switch0,  char* param0, int (*userfunc0)(char* x), int switch1,  int param1, int (*userfunc1)(int x), int startvararg, ...); 

#define TEST_FINISHED testCompleted()
void testCompleted();
#endif
