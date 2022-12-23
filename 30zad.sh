#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid arguments"
	exit 1
fi

logdir="${1}"

if [ ! -d "${logdir}" ]; then
	echo "Invalid directory name"
	exit 1
fi

if [ ! -r "${logdir}" ]; then
	echo "Directory not readable"
	exit 1
fi

temp=$(mktemp)

while read friend; do
	total_lines=0
	while read file; do
		file_lines=$(cat "${file}" | wc -l)
		total_lines=$(( total_lines + file_lines ))
	done < <(find "${logdir}" -mindepth 4 -maxdepth 4 -type f | fgrep "${friend}")
	
	echo "${friend}" "${total_lines}" >> "${temp}"
done < <(find "${logdir}" -mindepth 3 -maxdepth 3 -type d | cut -d '/' -f 4 | sort | uniq)

while read user lines; do
	echo "${user}" "${lines}"
done < <(cat "${temp}" | sort -rn -k 2 | head -n 10)

rm -- "${temp}"
