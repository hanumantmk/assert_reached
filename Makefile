CXX=clang++
CXXFLAGS=-Wno-undefined-internal -Wall -ggdb3 -O2 -std=c++14

OBJECTS=\
	assert_reached.o \
	example.o

default: example

$(OBJECTS): Makefile assert_reached.h

example: Makefile $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

%.o:%.c
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f example *.o
