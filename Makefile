GXX := g++
GCC := gcc
CXXFLAGS := -Wshadow -Wall
CFLAGS := -Wshadow -Wall

SYSTEM_UNDER_TEST := system_under_test
TEST_FILE := test_sut

.PHONY: testc testcc


result: temporary_file.o $(SYSTEM_UNDER_TEST).o  $(TEST_FILE).o helper.o 
	$(GXX) $(CXXFLAGS) -g -o result $(SYSTEM_UNDER_TEST).o temporary_file.o  $(TEST_FILE).o helper.o

$(TEST_FILE).o: $(TEST_FILE).c 
	$(GCC) $(CFLAGS) -g -c  $(TEST_FILE).c

temporary_file.o: temporary_file.cpp 
	$(GXX) $(CXXFLAGS) -c -g temporary_file.cpp 

temporary_file.cpp: testc

helper.o: helper.cpp helper.h
	$(GXX) $(CXXFLAGS) -g -c helper.cpp

$(SYSTEM_UNDER_TEST).o: $(SYSTEM_UNDER_TEST).c
	$(GCC) $(CFLAGS) -g -c $(SYSTEM_UNDER_TEST).c  

testc: hello.c plugin.so
	$(GCC) $(CFLAGS) -g -S -fplugin=./plugin.so $(SYSTEM_UNDER_TEST).c

plugin.so: plugin-3.cxx
	@echo Compiling plugin.so
	$(GXX) $(CXXFLAGS) -g -I`$(GXX) -print-file-name=plugin`/include -fPIC -shared plugin-3.cxx -o plugin.so

clean:
	rm -f plugin.so temporary_file.o temporary_file.cpp result temporary_file.h.gch temporary_file.h helper.o temporary_file.cpp~ temporary_file.h~ helper.h~ temporary_file.c~ myframework.h~ plugin-3.cxx~ helper.cpp~ $(SYSTEM_UNDER_TEST) $(TEST_FILE).o $(SYSTEM_UNDER_TEST).o 
