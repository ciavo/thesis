#ifndef TEMPORARYFILE_H
#define TEMPORARYFILE_H
#define EXPECT_foo(...) expect_foo( __VA_ARGS__, NO_MORE_PARAMETERS)
void expect_foo(int switch0, double param0, int (*userfunc0)(double x),...); 

#define EXPECT_func(...) expect_func( __VA_ARGS__, NO_MORE_PARAMETERS)
void expect_func(int switch0, double param0, int (*userfunc0)(double x), int switch1, int param1, int (*userfunc1)(int x),...); 

#endif
