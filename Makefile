CC=g++
RANLIB=ranlib

LIBOBJ=CacheFS.o

CFLAGS = -Wall -Wextra -g
LOADLIBES = -L./ 
LFLAGS = -o

TAR=tar
TARFLAGS=-cvf
TARNAME=ex2.tar
TARSRCS=$(LIBSRC) Makefile README Algorithm.h Block.cpp Block.h CacheFS.cpp FBRAlg.h LFUAlg.h LRUAlg.h myFile.cpp myFile.h

all: CacheFS.a

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

filereader: filereader.cpp
	$(CC) $^ -o $@
 
CacheFS.o: CacheFS.cpp CacheFS.h
	$(CC) -c $^ -o $@

CacheFS.a: $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@


clean:
	$(RM) $(TARGETS) $(OBJ) $(LIBOBJ) *~ *core

depend:
	makedepend -- $(CFLAGS) -- $(SRC)

tar:
	$(TAR) $(TARFLAGS) $(TARNAME) $(TARSRCS)

jona:
	mv -f Search jonas/Test_496/Search; \
	mv -f libMapReduceFramework.a jonas/Framework/MapReduceFramework.a; \
	cd jonas; \
	./compile_suite; \
	./run_suite; \
	cd ..;

