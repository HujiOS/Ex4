CC=g++
RANLIB=ranlib

LIBOBJ=CacheFS.o

CFLAGS = -Wall -Wextra -c -g
LOADLIBES = -L./
LFLAGS = -o

MORECLEAN=CacheFS.a *.o

TAR=tar
TARFLAGS=-cvf
TARNAME=ex4.tar
TARSRCS=$(LIBSRC) Makefile README Answers.pdf Algorithm.h Block.cpp Block.h CacheFS.cpp FBRAlg.h LFUAlg.h LRUAlg.h myFile.cpp myFile.h

all: CacheFS.a

TEST.o : TEST.cpp
	$(CC) $(CFLAGS) $^ -o $@

TEST : TEST.o
	$(CC) TEST.o $(LOADLIBES) CacheFS.a -o TEST

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

CacheFS.a: $(LIBOBJ) myFile.o Block.o
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean:
	$(RM) $(TARGETS) $(OBJ) $(LIBOBJ) $(MORECLEAN) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)

test1:
	g++ -c -Wall -std=c++11 -DNDEBUG Test1.cpp -o Test1.o
	g++ Test1.o -L. CacheFS.a -o Test1

jona:
	mv -f Search jonas/Test_496/Search; \
	mv -f libMapReduceFramework.a jonas/Framework/MapReduceFramework.a; \
	cd jonas; \
	./compile_suite; \
	./run_suite; \
	cd ..;

