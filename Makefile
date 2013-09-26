GXX := g++
GCC := gcc

.PHONY: testc testcc

all: testc hello.o temporary_file.o ris

ris: temporary_file.o hello.o
	$(GCC) -o ris hello.o temporary_file.o

temporary_file.o: temporary_file.c
	$(GCC) -c temporary_file.c

hello.o: hello.c
	$(GCC) -c hello.c

testc: hello.c plugin.so
	$(GCC) -g -S -fplugin=./plugin.so hello.c

testcc: hello.c plugin.so
	$(GXX) -g -S -fplugin=./plugin.so hello.c

plugin.so: plugin-3.cxx
	@echo Compiling plugin.so
	$(GXX) -g -I`$(GXX) -print-file-name=plugin`/include -fPIC -shared plugin-3.cxx -o plugin.so

