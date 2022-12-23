#!/bin/bash

dir="${1}"
name=0
if [ $# -eq 2 ]; then
	name="${2}"
fi

cnt=0

while read link; do
	file="$(readlink -f "${link}")"
	if [ $(file "${link}" | grep 'broken' | wc -l) ]; then
		cnt=$(( cnt + 1 ))
	fi

	if [ $# -eq 2 ]; then
		echo ""${link}"	-> "${file}"" >> "${name}"
	else
		echo ""${link}" -> "${file}""
	fi
done < <(find "${dir}" -type l)

echo "Broken symlinks: "${cnt}""
