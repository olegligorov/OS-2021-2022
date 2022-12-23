#!/bin/bash

[ $? -eq 3 ] || { echo "invalid number of arguments"; exit 1; }

file1="${1}"
file2="${2}"

file1count=$(cat "${file1}" | grep "${file1}" | wc -l)
file2count=$(cat "${file2}" | grep "${file2}" | wc -l)

file="${file1}"
if [[ "${file1count}" -gt "${file2count}" ]]; then
	file="${file1}"
else
	file="${file2}"
fi

cat "${file}" | cut -d ' ' -f 4- | sort > ""${file}".songs"
