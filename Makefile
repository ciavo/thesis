GXX := g++
GCC := gcc
CXXFLAGS := -Wshadow

.PHONY: testc testcc


result: temporary_file.o hello.o testhello.o helper.o 
	$(GXX) -o result hello.o temporary_file.o testhello.o helper.o

testhello.o: testhello.c 
	$(GCC) -c testhello.c

temporary_file.o: temporary_file.cpp 
	$(GXX) -c temporary_file.cpp 

temporary_file.cpp: testc

helper.o: helper.cpp helper.h
	$(GXX) -c helper.cpp

hello.o: hello.c
	$(GCC) -c hello.c 

testc: hello.c plugin.so
	$(GCC) -g -S -fplugin=./plugin.so hello.c

plugin.so: plugin-3.cxx
	@echo Compiling plugin.so
	$(GXX) $(CXXFLAGS) -g -I`$(GXX) -print-file-name=plugin`/include -fPIC -shared plugin-3.cxx -o plugin.so

clean:
	rm -f plugin.so hello.o temporary_file.o temporary_file.cpp testhello.o result temporary_file.h.gch temporary_file.h 
