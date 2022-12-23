#!/bin/bash

if [ $# -eq 1 ]; then
	echo "invalid number of arguments"
	exit 1
fi

D="${1}"

if [ ! -d "${d}" ]; then
	echo "Directory does not exist"
	exit 1
fi

while read smlink; do
	if [ $(file "${smlink}" | grep 'broken') ]; then
		echo "${smlink}"
	fi	
done < <(find "${dir}" -type l)

find 
