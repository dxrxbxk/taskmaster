#!/usr/bin/env -S zsh --no-rcs --no-globalrcs --pipefail



declare -rg list=(
	'more secure lock file'
	'config parser'
	'main argument parser'
	'data structure for tasks'
	'hot reload config'
)

echo -n '\x1b[32m'
for task in $list; do
	printf '- %s\n' $task
done
echo -n '\x1b[0m'
