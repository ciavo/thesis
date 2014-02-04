© 2014 Luca Ciavaglia. All rights reserved

Master Thesis - Using compiler plugins for mocking C code

Abstract

For any software it is important that it is possible to write tests to ensure that the
software is working as intended; but when the software is large, it is convenient
test part of it in isolation. The goal of this project is to make isolated testing of
individual C files easy by using GCC plugins. Any function calls that are made
from the tested file to an outside function should be mocked automatically, using
information that is available in the compiler.
It is possible through plugins, to access at the abstract syntax tree (AST) built
when GCC parses the input file. In the AST we can find all the information necessary
to mock such functions. To perfom automatic mocking, the external function of the
software to test are replaced with the new functions generated via plugin.
In this thesis, we explain how we have developed a system able to perfom
automatic mocking.

The thesis has been completely realized at the Baseband Tools department of
Ericsson AB in Linköping, Sweden.

For more detail, download the pdf file.

Project tested with GCC 4.7.2
