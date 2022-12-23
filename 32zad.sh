#!/bin/bash

file1="${1}"
file2="${2}"

fieldcount=$(cat "${file1}" | head -n 1 | awk -F ',' '{print NF}')

touch "${file2}"

while read line; do
	ID=$(echo "${line}" | cut -d ',' -f 1)
	rest=$(echo "${line}" | cut -d ',' -f 2-)

	if [[ $(cat "${file2}" | fgrep ","${rest}"" | wc -l) -gt 0 ]]; then
		continue
	fi

	echo "${line}" >> "${file2}"

done < <(cat "${file1}" | sort -n -t ',' -k 1)
