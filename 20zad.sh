#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid arguments"
	exit 1
fi

file="${1}"

while read str; do
	lineid=$(echo "${str}" | cut -d ' ' -f 1)
	rest=$(echo "${str}" | cut -d ' ' -f 2-)
	echo "${lineid}". "${rest}"
done < <(cat "${file}" | cut -d ' ' -f 4- | grep -n ' ' | tr ':' ' ' | sort -k 2)
