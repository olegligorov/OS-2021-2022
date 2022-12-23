#!/bin/bash

dir="${2}"
file="${1}"

if [ $(ls "${dir}" | wc -l) -gt 0 ]; then
	echo "The directory is not empty"
	exit 1
fi

count=1

touch "${dir}/dict.txt"

while read str; do
	name="$(echo "${str}" | cut -d ':' -f 1 |  cut -d '(' -f 1 | tr -s ' ')"
	number="$(cat "${dir}/dict.txt" | fgrep "${name}" | cut -d ';' -f 2)"

	if [ -z "${number}" ]; then
		#if the number does not exist
		echo "${name}";"${count}" >> "${dir}/dict.txt"
		number="${count}"
		count=$(( count + 1 ))
	fi
	
	touch "${dir}/${number}.txt"
	echo "${str}" | cut -d ':' -f 2 >> "${dir}/${number}.txt"
done < <(cat "${file}")
