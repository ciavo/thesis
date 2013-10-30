GXX := g++
GCC := gcc
CXXFLAGS := -Wshadow -Wall
CFLAGS := -Wshadow -Wall
.PHONY: testc testcc


result: temporary_file.o hello.o testhello.o helper.o 
	$(GXX) $(CXXFLAGS) -g -o result hello.o temporary_file.o testhello.o helper.o

testhello.o: testhello.c 
	$(GCC) $(CFLAGS) -g -c testhello.c

temporary_file.o: temporary_file.cpp 
	$(GXX) $(CXXFLAGS) -c -g temporary_file.cpp 

temporary_file.cpp: testc

helper.o: helper.cpp helper.h
	$(GXX) $(CXXFLAGS) -g -c helper.cpp

hello.o: hello.c
	$(GCC) $(CFLAGS) -g -c hello.c 

testc: hello.c plugin.so
	$(GCC) $(CFLAGS) -g -S -fplugin=./plugin.so hello.c

plugin.so: plugin-3.cxx
	@echo Compiling plugin.so
	$(GXX) $(CXXFLAGS) -g -I`$(GXX) -print-file-name=plugin`/include -fPIC -shared plugin-3.cxx -o plugin.so

clean:
	rm -f plugin.so hello.o temporary_file.o temporary_file.cpp testhello.o result temporary_file.h.gch temporary_file.h helper.o temporary_file.cpp~ temporary_file.h~ helper.h~ hello.h~ hello.c~ testhello.c~ temporary_file.c~ myframework.h~ plugin-3.cxx~ helper.cpp~ test.c~
