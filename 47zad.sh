#!/bin/bash

if [ $# -ne 2 ]; then
	echo "2 arguments are needed"
	exit 1
fi

dir1="${1}"
dir2="${2}"

if [[ $(ls "${dir2}" | wc -l) -ne 0 ]]; then
	echo "${dir2} is not empty"
	exit 1
fi

while read file; do
	dirname=$(dirname "${file}" | sed "s/${dir1}/${dir2}/") 
	
	if [ ! -d "${dirname}" ]; then
		mkdir -p "${dirname}"
	fi
	
	cp "${file}" "${dirname}"
done < <(find "${dir1}" -type f | grep -v '.swp')
