#!/bin/bash

dir="${1}"
if [ ! -d "${dir}" ]; then
	echo "The directory is invalid"
	exit 1
fi

if [ ! -r "${dir}" ]; then
	echo "Can't read in the directory"
	exit 1
fi

temp=$(mktemp)
lastran=$(find -name "${0}" -printf '%T@')

while read archive; do
	timestamp=$(echo "${archive}" | cut -d '-' -f 2 | cut -d '.' -f 1)
	if [ "${timestamp}" -gt "${lastran}" ]; then
		
	fi		
done < <(find "${dir}" | egrep '.*_report.*\.tgz')
