#!/bin/bash

file="${1}"
key="${2}"
value="${3}"

temp=$(mktemp)

while read line; do
	if [[ $(echo "${line}" | grep "${key}" | wc -l) -eq 0 ]]; then
		echo "${line}" >> "${temp}"
		continue	
	fi

	val=$(echo "${line}" | cut -d '#' -f 1 | cut -d '=' -f 2- | sed 's/ //')
	
	if [[ "${value}" =~ "${val}" ]]; then
		echo "${line}" >> "${temp}"
		continue
	fi	
	
	oldmessage="#""${line}"" # edited at $(date) by $(whoami)"
	newmessage="${key} = ${value} # added at $(date) by $(whoami)"
	
	echo "${oldmessage}" >> "${temp}"
	echo "${newmessage}" >> "${temp}"

done < "${file}"

cat "${temp}" > "${file}"

rm "${temp}"
