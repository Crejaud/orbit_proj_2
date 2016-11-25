
CXX=clang++ -m64
CXXFLAGS=-Iobjs/ -O3 -Wall
ISPC=ispc
ISPCFLAGS=-O2 -g --arch=x86-64 --target=avx2-i32x8

default: sqrt

.PHONY: dirs clean
.PRECIOUS: objs/sqrt.h

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ sqrt

sqrt:
	/bin/mkdir -p objs/
	ispc $(ISPCFLAGS) sqrt.ispc -o objs/sqrt_ispc.o -h objs/sqrt_ispc.h
	clang++-3.5 -m64 -g -mavx -Iobjs/ -O3 -Wall -c -o objs/sqrt.o sqrt.cpp
	clang++-3.5 -m64 -g -mavx -lobjs/ -O3 -Wall -c -o objs/task.o tasksys.cpp
	clang++-3.5 -m64 -g -mavx -Iobjs/ -O3 -Wall -o sqrt objs/sqrt.o objs/sqrt_ispc.o objs/task.o -lpthread
