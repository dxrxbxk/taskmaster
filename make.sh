#!/usr/bin/env -S zsh --no-rcs --no-globalrcs --pipefail


# -- C O L O R S --------------------------------------------------------------

declare -rg success='\x1b[32m'
declare -rg   error='\x1b[31m'
declare -rg warning='\x1b[33m'
declare -rg    info='\x1b[34m'
declare -rg     dim='\x1b[90m'
declare -rg   reset='\x1b[0m'

# print success message
function _success() {
	echo $success'[✓]'$reset $@
}

# print error message
function _error() {
	echo $error'[x]'$reset $@
}

# print warning message
function _warning() {
	echo $warning'[!]'$reset $@
}

# print info message
function _info() {
	echo $info'[>]'$reset $@
}


# -- L O G O ------------------------------------------------------------------

echo $success'\n' \
'▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖ \n' \
'  █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌\n' \
'  █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖\n' \
'  █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌\n' \
$reset


# -- T H I S  S C R I P T -----------------------------------------------------

# get script absolute directory path
declare -rg cwd_dir=${${0%/*}:a}

# get script absolute path
declare -rg script=${0:a}


# -- T A R G E T S ------------------------------------------------------------

# main executable
declare -rg executable=$cwd_dir'/taskmaster'

# compile commands database
declare -rg compile_db=$cwd_dir'/compile_commands.json'

# log file
declare -rg logfile='/var/log/taskmaster/taskmaster.log'


# -- D I R E C T O R I E S ----------------------------------------------------

# source directory
declare -rg src_dir=$cwd_dir'/sources'

# include directory
declare -rg inc_dir=$cwd_dir'/include'


# -- F I L E S ----------------------------------------------------------------

# source files
declare -rg srcs=($src_dir'/'**'/'*'.cpp'(.N))

# object files
declare -rg objs=(${srcs/%.cpp/.o})

# dependency files
declare -rg deps=(${srcs/%.cpp/.d})



# -- T O O L S ----------------------------------------------------------------

# function to check required tools
function _check_tools() {

	# required tools
	local -r required=('rm' 'nproc' 'wait' 'clang++')
	
	# optional tools
	local -r optional=('ccache' 'fzy')

	# loop over required tools
	for tool in $required; do

		# check if tool is available
		if ! command -v $tool > '/dev/null'; then
			echo 'required tool' $error$tool$reset 'not found.'
			exit 1
		fi
	done

	# loop over optional tools
	for tool in $optional; do

		# check if tool is available
		if ! command -v $tool > '/dev/null'; then
			echo 'optional tool' $warning$tool$reset 'not found.'
		fi
	done
}

# check tools
_check_tools



# -- C O M P I L E R  S E T T I N G S -----------------------------------------

# compiler
if ! command -v 'ccache' > '/dev/null'; then
	declare -rg cxx='clang++'
else
	declare -rg cxx=('ccache' 'clang++')
fi

# cxx flags
declare -rg cxxflags=('-std=c++2a' '-O0'
					  '-g3' '-gdwarf-4'
					  #'-fsanitize=address'
					  '-DENGINE_VL_DEBUG'
					  '-Wall' '-Wextra' '-Werror' '-Wpedantic' '-Weffc++'
					  '-ferror-limit=1'
					  '-fno-rtti' '-Winline'
					  '-Wno-unused' '-Wno-unused-variable' '-Wno-unused-parameter'
					  '-Wno-unused-function' '-Wno-unused-private-field' '-Wno-unused-local-typedef'
					  '-Wconversion' '-Wsign-conversion' '-Wfloat-conversion' '-Wnarrowing'
					  '-fdiagnostics-color=always'
					  '-fno-diagnostics-show-note-include-stack'
					  '-fdiagnostics-show-location=once'
					  '-fdiagnostics-show-template-tree'
					  '-Wshadow'
					  '-I'$inc_dir
				)

# linker flags
declare -rg ldflags=( -lreadline
				#-fsanitize=address
			)



# -- C O M P I L E  D A T A B A S E -------------------------------------------

# generate compile database
function _generate_compile_db() {

	local content='[\n'

	for file in $srcs; do

		# current directory
		content+='\t{\n\t\t"directory": "'$cwd_dir'",'

		# source file
		content+='\n\t\t"file": "'$file'",'

		# output file
		content+='\n\t\t"output": "'${file%.cpp}'.o",'

		# arguments
		content+='\n\t\t"arguments": [\n\t\t\t"'$cxx'",\n'

		# cxx flags
		for flag in $cxxflags; do
			content+='\t\t\t"'$flag'",\n'
		done

		# source file
		content+='\t\t\t"-c",\n\t\t\t"'$file'",\n'

		# output file
		content+='\t\t\t"-o",\n\t\t\t"'${file%.cpp}'.o"\n\t\t]\n\t},\n'
	done

	# erase last comma with newline
	content[-3]='\'
	content[-2]='n'
	content[-1]=']'

	# write to compile db
	if ! echo $content > $compile_db; then
		echo 'error while generating' $error'compile_commands.json'$reset
		exit 1
	fi

	# print success
	print $success'[✓]'$reset ${compile_db:t}
}


# compile db
function _compile_database() {

	# check compile db exists and is up to date
	if [[ ! -f $compile_db ]] || [[ $compile_db -ot $script ]]; then
		_generate_compile_db
		return
	fi

	# loop over source files
	for src in $srcs; do

		# check if src is newer than compile db
		if [[ $src -nt $compile_db ]]; then
			_generate_compile_db
			break
		fi

	done
}



# -- C O M P I L A T I O N ----------------------------------------------------

function _check_dependency {
	# check if object or dependency file is missing
	if [[ ! -f $1 ]] || [[ ! -f $2 ]] || [[ $script -nt $1 ]]; then
		return 0
	fi
	# loop over words in dependency file
	for word in ${=$(<$2)}; do
		# check if word is not target or escape
		if [[ $word != '\' ]] && [[ $word != *':' ]]; then
			# check if dependency is missing
			[[ $word -nt $1 ]] && return 0
		fi
	done
	return 1
}


function _handle_compilation {

	# $1 source file
	# $2 object file
	# $3 dependency file
	# $4 character to print

	# compile source file
	$cxx $cxxflags -MT $2 -MMD -MF $3 -c $1 -o $2

	# check if compilation failed
	if [[ $? -ne 0 ]]; then
		echo -n $error'[x]'$reset
		exit 1
	fi

	echo -n '\r\x1b[2K'$success'['$4']'$reset ${1:t}
	exit 0
}

function _wait_processes {

	# loop over pids
	for pid in $@; do
		# wait for pid
		wait $pid
		# check if compilation failed
		if [[ $? -ne 0 ]]; then
			wait
			exit 1
		fi
	done
}


function _compile {

	# maximum number of jobs
	local -r max_jobs=$('nproc' '--all')

	# number of compiled files
	local count=0

	# array of pids
	local pids=()

	# array of characters
	local -r chars=('|' '/' '-' '\')

	# iterator
	local -i i=1

	# loop over source files
	for src in $srcs; do

		# add object file extension
		local obj=${src%.cpp}'.o'

		# add dependency file extension
		local dep=${src%.cpp}'.d'

		# check if source file is modified
		if ! _check_dependency $obj $dep; then
			continue
		fi

		if [[ ${#pids[@]} -eq $max_jobs ]]; then
			_wait_processes $pids
			pids=()
		fi

		count=$((count + 1))

		_handle_compilation $src $obj $dep ${chars[$i]} &

		# increment iterator
		i=$((i + 1))
		[[ $i -eq 5 ]] && i=1

		pids+=($!)
	done

	# wait for remaining pids
	_wait_processes $pids

	if [[ $count -ne 0 ]]; then
		echo '\r\x1b[2K'$success'[✓]'$reset $count 'files compiled'
	fi
}


# -- L I N K  F U N C T I O N S -----------------------------------------------

function _link_executable {

	# link object files
	if $cxx $objs '-o' $executable $ldflags; then
		echo $success'[✓]'$reset 'linked' ${executable:t}
	else
		echo $error'[x]'$reset 'linking failed'
		exit 1
	fi
}

function _link {

	# check if executable is missing
	if [[ ! -f $executable ]]; then
		_link_executable
	fi

	# loop over object files
	for obj in $objs; do

		# check if object file is newer than target
		if [[ $obj -nt $executable ]]; then
			_link_executable
			break
		fi
	done
}



# build
function _build() {

	# generate compile database
	_compile_database

	# build
	_compile
	_link
}


# clean
function _clean() {

	# remove all build files
	local -r deleted=($(rm -vrf $objs $deps \
								$executable \
								$compile_db \
								'.cache'))

	# get count of deleted files
	local -r count=${#deleted[@]}

	# check if files were deleted
	if [[ $count -ne 0 ]]; then
		echo $info'[x]'$reset $count 'files deleted.'
		return
	fi

	# nothing to clean
	echo $info'[>]'$reset 'nothing to clean.'
}


function _launch_test() {


	# create new tmux session
	tmux new-session -d -s 'taskmaster'

	# split window
	tmux split-window -h -t 'taskmaster'

	# send first command
	tmux send-keys -t 'taskmaster:0.0' "$taskmaster && clear && tail --follow $logfile" C-m

	# send second command
	tmux send-keys -t 'taskmaster:0.1' "sleep 1 && $taskcontrol" C-m

	# attach to session
	tmux attach-session -t 'taskmaster'
}

# kill
function _kill() {

	# get taskmaster pids
	local -r pids=($(ps -C 'taskmaster' -o pid=))

	# check if no taskmaster process
	if [[ ${#pids[@]} -eq 0 ]]; then
		_info 'no taskmaster process to kill.'
		return
	fi

	# check if multiple taskmaster processes
	if [[ ${#pids[@]} -gt 1 ]]; then
		_warning 'multiple taskmaster processes found.'
		return
	fi


	# default signal
	local sig=''

	# check if no arguments
	if [[ $# -eq 0 ]]; then

		if ! command -v 'fzy' > '/dev/null'; then
			_error 'fzy not found.'
			return
		fi

		local -r sigs=($(kill -l))

		IFS=$'\n '

		sig=$(fzy <<< $sigs)

		if [[ -z $sig ]]; then
			_error 'no signal selected.'
			return
		fi
	fi

	# check if there is argument
	if [[ $# -eq 1 ]]; then
		sig=$1
	fi

	# kill taskmaster process
	if ! kill -s $sig $pids; then
		_error 'failed to send' $sig 'to taskmaster process.'
		return
	fi

	# print success message
	_success 'sent' $sig 'to taskmaster process.'
}

# log
function _log() {

	# check if log file exists
	if [[ ! -f $logfile ]]; then
		_error 'log file not found.'
		return
	fi

	clear && tail --follow $logfile
}

# leak
function _leak() {

	# valgrind log file
	local -r valgrind_log='valgrind.log'

	# valgrind flags
	local -r valgrind_flags=(
				'--leak-check=full'
				'--show-leak-kinds=all'
				'--track-origins=yes'
				'--errors-for-leak-kinds=all'
				'--undef-value-errors=yes'
				'--malloc-fill=0x55'
				'--free-fill=0x55'
				'--track-fds=yes'
	)

	# run valgrind
	valgrind $valgrind_flags $taskmaster
}


# -- M A I N ------------------------------------------------------------------

# check if no arguments
if [[ $# -eq 0 ]]; then
	_build
	exit
fi

# handle arguments
case $1 in

	# clean
	'clean' | 'fclean' | 'rm')
		_clean
		;;

	# re
	're' | 'rebuild' | 'remake')
		_clean
		_build
		;;

	# test
	'test')
		_build
		_launch_test
		;;
	
	# kill
	'kill')
		shift
		_kill $@
		;;

	# log
	'log')
		_log
		;;

	# leak
	'leak')
		_build
		_leak
		;;

	# unknown (usage)
	*)
		echo 'usage:' './'${script:t} '[clean|fclean|rm|re]'
		;;
esac
