#!/usr/bin/env -S zsh --no-rcs --no-globalrcs --pipefail


declare -rg lib_name='tomlplusplus'
declare -rg cwd_dir=${0:a:h}
declare -rg repo_url='https://github.com/marzer/'$lib_name'.git'
declare -rg repo_name=$cwd_dir'/'$lib_name
declare -rg branch='master'
declare -rg install_dir=$cwd_dir'/external/'$lib_name

# check if the repo is already cloned
if [[ ! -d $repo_name ]]; then

	# clone the repository
	if ! git clone --depth 1 --branch $branch --single-branch $repo_url $repo_name; then
		echo 'Failed to clone the toml++ repository'
		exit 1
	fi
fi

# create the install directory
mkdir -p $install_dir

# cmake flags
declare -rg cmake_flags=('-DCMAKE_BUILD_TYPE=Release'
						 '-DCMAKE_INSTALL_PREFIX='$install_dir)

# build the project
if ! cmake -S $repo_name -B $repo_name'/build' -G 'Unix Makefiles' $cmake_flags; then
	echo 'Failed to configure the toml++ project'
	rm -rf $repo_name
	exit 1
fi

# install the project
if ! make --directory=$repo_name'/build' 'install'; then
	echo 'Failed to build the toml++ project'
	rm -rf $repo_name
	exit 1
fi

# remove the repository
rm -rf $repo_name
