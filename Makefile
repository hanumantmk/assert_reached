CXX=clang++
CXXFLAGS=-Wno-undefined-internal -Wall -ggdb3 -O2 -std=c++14

OBJECTS=\
	assert_reached.o \
	custom.o \
	example.o

default: example custom

$(OBJECTS): Makefile assert_reached.h

example: Makefile $(OBJECTS)
	$(CXX) $(CXXFLAGS) assert_reached.o example.o -o $@

custom: Makefile $(OBJECTS)
	$(CXX) $(CXXFLAGS) assert_reached.o custom.o -o $@

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f custom example *.o
