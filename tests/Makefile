
override srcs := $(shell find '.' -type f -name '*.cpp')
override exes := $(srcs:%.cpp=%.exe)


.PHONY: all


all: $(exes)


%.exe: %.cpp
	clang++ -std=c++2a -Wall -Wextra -Wpedantic -Werror -o $@ $<

clean:
	rm -f $(exes)
