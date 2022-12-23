#!/bin/bash

[ $? -eq 3 ] || { echo "Invalid number of arguments"; exit 1;}

num1=${1}
num2=${2}

mkdir a
mkdir b
mkdir c

while read filename; do
	lines=$(cat "${filename}" | wc -l)
	if [[ "${num1}" -gt "${lines}" ]]; then
		mv "${filename}" a/
	elif [[ "${num2}" -gt "${lines}" ]]; then
		mv "${filename}" b/
	else
		mv "${filename}" c/
	fi
done > >(ls)
