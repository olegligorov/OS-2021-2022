#!/bin/bash

dir="${1}"
num=0

if [ $# -eq 2 ]; then
	num="${2}"
elif [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

if [ ! -d "${dir}" ]; then
	echo "Not a valid directory name"
	exit 3
elif [ ! -r "${dir}" ]; then
	echo "Directory is not readable"
	exit 3
fi

if [ $# -eq 2 ]; then
	while read f; do
		num_of_hardlinks=$(find "${dir}" -samefile "${f}" | wc -l)
		if [[ "${num_of_hardlinks}" -ge "${num}" ]]; then
			echo "${f}"
		fi
	done < <(find "${dir}" -type f)
else
	while read l; do
		if [[ $(file "${l}" | grep 'broken') ]]; then
			echo "${l}"
		fi
	done < <(find "${dir}" -type l)
fi
