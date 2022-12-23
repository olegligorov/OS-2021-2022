#!/bin/bash

if [ $# -ne 3 ]; then
	echo "There should be 3 arguments"
	exit 1
fi

file1="${1}"
file2="${2}"
dir="${3}"

valid=$(mktemp)
while read str; do
	#error="$(egrep -nv "^(#.*|\{.*\};)$" "$str")"
	error="$(cat $str | egrep -nv "^{.*};$" | egrep -nv "^#")"

	if [ ! -z "${error}" ]; then
		echo "Error in $str: "
	else
		echo "${str}" >> "${valid}"
		continue
	fi
	
	awk -F ':' '{printf "Line "$1": "$2"\n" }' <<< ${error}
	echo ""
done < <(find "${dir}" -type f | grep '.cfg')

while read line; do
	cat "${line}" >> "${file2}"

	name="$(basename "${line}" | cut -d '.' -f 1)"
	if [[ $(cat "${file1}" | fgrep "${name}" | wc -l) -eq 0 ]]; then
		generated_pw=$(pwgen 16,1)
		echo "$name:${generated_pw}" >> "${file1}"
		echo "$name:${generated_pw}"
	fi

done < "${valid}"

rm -- ${valid}
