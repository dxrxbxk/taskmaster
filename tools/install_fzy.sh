#!/usr/bin/env -S zsh --no-rcs --no-globalrcs


# -- C H E C K  R O O T  P R I V I L E G E S ----------------------------------

# require root privileges
if [[ $EUID -ne 0 ]]; then
	echo 'error: this script requires root privileges'
	exit 1
fi


# -- R E Q U I R E M E N T S --------------------------------------------------

declare -r required=('make' 'git')

# loop over the required packages
for package in $required; do

	# check if the package is installed
	if ! command -v $package &> '/dev/null'; then
		echo "error: $package is not installed"
		exit 1
	fi
done



# -- V A R I A B L E S --------------------------------------------------------

# script directory
declare -r cur_dir=${${0%/*}:a}

# repository url
declare -r repo_url='https://github.com/jhawthorn/fzy.git'

# repository directory
declare -r repo_dir=$cur_dir'/fzy_repository'

# branch
declare -r branch='master'


# -- C L E A N  U P -----------------------------------------------------------

function cleanup() {

	# remove the repository
	rm -rf $repo_dir
}


# -- C L O N E  R E P O S I T O R Y -------------------------------------------

# check if the repository already exists
if [[ ! -d $repo_dir ]]; then

	# clone the repository
	if ! git clone --branch $branch --single-branch --depth 1 $repo_url $repo_dir; then
		echo 'error: failed to clone the repository'
		cleanup
		exit 1
	fi
fi


# make flags
local make_flags=()

# check if too many arguments are passed
if [[ $# -gt 1 ]]; then
	echo 'usage: install_fzy.sh [install directory]'
	exit 1
fi

# check if one argument is passed
if [[ $# -eq 1 ]]; then

	# add default flags
	make_flags+=('PREFIX='$1)

	# create the install directory
	mkdir -p $1
fi

# build the project
if ! make --directory=$repo_dir $make_flags; then
	echo 'error: failed to build the project'
	cleanup
	exit 1
fi

# install the project
if ! make --directory=$repo_dir $make_flags install; then
	echo 'error: failed to install the project'
	cleanup
	exit 1
fi


# cleanup
cleanup

# exit
exit 0
