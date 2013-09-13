GXX := g++
GCC := gcc

.PHONY: testc testcc

testc: hello.c plugin.so
	$(GCC) -g -S -fplugin=./plugin.so hello.c

testcc: hello.c plugin.so
	$(GXX) -g -S -fplugin=./plugin.so hello.c

plugin.so: plugin-3.cxx
	@echo Compiling plugin.so
	$(GXX) -g -I`$(GXX) -print-file-name=plugin`/include -fPIC -shared plugin-3.cxx -o plugin.so

