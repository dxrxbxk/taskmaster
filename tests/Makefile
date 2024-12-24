override srcs := $(shell find '.' -type f -name '*.cpp')
override asms := $(shell find '.' -type f -name '*.s')

override exes := $(srcs:%.cpp=%.exe)
override asm_objs := $(asms:%.s=%.o)
override asm_exes := $(asms:%.s=%.exe)

override CXXFLAGS := -std=c++2a -Wall -Wextra -Wpedantic -Werror
override ASFLAGS := -f elf64

.PHONY: all clean re

all: $(exes) $(asm_exes)

%.exe: %.cpp
	clang++ -std=c++2a -Wall -Wextra -Wpedantic -Werror -o $@ $< 

%.o: %.s
	nasm -f elf64 -o $@ $<


%.exe: %.o
	clang++ -o $@ $^ -nostartfiles

clean:
	rm -f $(exes) $(asm_exes) $(asm_objs)

re: clean all
