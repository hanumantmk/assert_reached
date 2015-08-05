default: example

example: example.cpp assert_reached.h Makefile
	clang++ -Wno-undefined-internal -Wall -ggdb3 -O2 -std=c++14 -o example example.cpp

clean:
	rm -f example
