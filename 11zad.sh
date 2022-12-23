#!/bin/bash

required_inode=$(find ~velin/ -type f -printf '%T@ %i %p' | sort -k 1 -rn | head -n 1 | cut -d ' ' -f 2)
min_depth=10000000
name=0
while read str; do

#	depth=$(echo "${str}" | awk -F '/' '{print NF}')
#	depth=$(( $depth - 1 ))
	depth="$( echo "${str}" | grep -o '/' | wc -l)"

	if [[ ${min_depth} -gt ${depth} ]]; then
		min_depth="${depth}"
		name="${str}"
	fi
done < <(find ~velin/ -type f -printf '%i %p' | grep "${required_inode}" | cut -d ' ' -f 2)
echo "${name}"
