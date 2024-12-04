
# -- S E T T I N G S ----------------------------------------------------------

# set default target
.DEFAULT_GOAL := all

# set shell program
override SHELL := $(shell which zsh)

# shell flags
.SHELLFLAGS := -d -f -c -e -o pipefail -u


# -- O P E R A T I N G  S Y S T E M -------------------------------------------

override os := $(shell uname -s)

ifeq ($(os), Darwin)
override threads := $(shell sysctl -n hw.logicalcpu)
endif
ifeq ($(os), Linux)
override threads := $(shell nproc --all)
endif

# set make flags
override MAKEFLAGS += --warn-undefined-variables --no-builtin-rules \
					  --jobs=1 --output-sync=target \
					  --no-print-directory


# -- T A R G E T S ------------------------------------------------------------

# project name
override project := taskmaster

# main executable
override executable := $(project)

# compile commands database
override compile_db := compile_commands.json



# -- D I R E C T O R I E S ----------------------------------------------------

# root directory
override cwd_dir := $(CURDIR)

# source directory
override src_dir := $(cwd_dir)/sources

# include directory
override inc_dir := $(cwd_dir)/include


# -- F I L E S ----------------------------------------------------------------

# source files
override srcs := $(shell find $(src_dir) -type f -name '*.cpp')

# object files
override objs := $(srcs:%.cpp=%.o)

# dependency files
override deps := $(objs:%.o=%.d)


# -- C O M P I L E R  S E T T I N G S -----------------------------------------

# compiler
ifeq ($(shell command -v ccache),)
override cxx := clang++
else
override cxx := ccache clang++
endif

# compiler standard
override std := -std=c++2a

# compiler optimization
override opt := -O0

# debug flags
override debug := -g2

# defines
override defines ?=

# include flags
override includes := -I$(inc_dir) 

# cxx flags
override cxxflags := $(std) $(opt) $(debug) $(defines) $(includes) \
					 -Wall -Wextra -Werror -Wpedantic \
					 -fno-rtti -Weffc++ -Winline \
					 -Wno-unused -Wno-unused-variable -Wno-unused-parameter \
					 -Wno-unused-function -Wno-unused-private-field -Wno-unused-local-typedef \
					 -Wconversion -Wsign-conversion -Wfloat-conversion -Wnarrowing \
					 -Wshadow \
					 -fdiagnostics-color=always

# dependency flags
override depflags = -MT $@ -MMD -MP -MF $*.d

# linker flags
override ldflags ?=



# -- P H O N Y  T A R G E T S -------------------------------------------------

# use one shell for all commands
.ONESHELL:

# delete intermediate files on error
.DELETE_ON_ERROR:

# silent mode
.SILENT:

# non file targets
.PHONY: all clean fclean re objs shaders xns ascii

# not parallel
.NOTPARALLEL: $(compile_db) re fclean clean ascii



# -- R U L E S ----------------------------------------------------------------

ascii:
	echo '\033[32m\n' \
	'▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖ \n' \
	'  █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌\n' \
	'  █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖\n' \
	'  █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌\n' \
	'\033[0m'


all: ascii $(executable) $(compile_db)

# executable
$(executable): $(objs)
	$(cxx) $^ -o $@ $(ldflags)
	$(call print,36,exe,$@)

# compilation
-include $(deps)
%.o: %.cpp Makefile
	$(cxx) $(cxxflags) $(depflags) -c $< -o $@
	$(call print,32,cxx,$(@F))

# compile commands
$(compile_db): $(srcs) Makefile
	$(call generate_cdb)
	$(call print,34,cdb,$@)

# clean
clean:
	rm -vfr $(objs) $(deps) $(compile_db) '.cache'

# fclean
fclean: clean
	rm -vf $(executable)

# re
re: fclean all


# -- F O R M A T T I N G ------------------------------------------------------

define print
echo '\033[90m[\033[$(1)m'$(2)'\033[0m\033[90m]\033[0m' '$(3)'
endef



# generate compile_commands.json
define generate_cdb
content='[\n'
for file in $(srcs); do
content+='\t{\n\t\t"directory": "'$$(pwd)'",\n\t\t"file": "'$$file'",\n\t\t"output": "'$${file%.cpp}.o'",\n\t\t"arguments": [\n\t\t\t"$(cxx)",\n'
	for flag in $(cxxflags); do
		content+='\t\t\t"'$$flag'",\n'
	done
	content+='\t\t\t"-c",\n\t\t\t"'$$file'",\n\t\t\t"-o",\n\t\t\t"'$${file%.cpp}'.o"\n\t\t]\n\t},\n'
done
echo $${content%',\n'}'\n]' > $@
endef
