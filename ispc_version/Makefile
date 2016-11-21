
CXX=clang++ -m64
CXXFLAGS=-Iobjs/ -O3 -Wall
ISPC=ispc
ISPCFLAGS=-O2 --arch=x86-64 --target=sse2

default: sqrt

.PHONY: dirs clean
.PRECIOUS: objs/sqrt.h

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ sqrt

simple: dirs  objs/sqrt.o objs/sqrt_ispc.o
	$(CXX) $(CXXFLAGS) -o $@ objs/sqrt.o objs/sqrt_ispc.o

objs/simple.o: sqrt.c objs/sqr_ispc.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h
