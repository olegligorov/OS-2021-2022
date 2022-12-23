#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid usage"
	exit 1
fi

dir="${1}"
str="${2}"

if [ ! -d "${dir}" ]; then
	echo "Invalid directory"
	exit 1
elif [ ! -r "${dir}" ]; then
	echo "directory is not readable"
	exit 1
fi

temp=$(mktemp)

find "${dir}" -mindepth 1 -maxdepth 1 -type f -printf '%f\n' | grep -E "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-"${str}"">"${temp}"

searched="$(cat "${temp}" | cut -d '-' -f 2 | sort -t '.' -n -k 1,1 -k 2,2 -k 3,3 | tail -n 1)"

fgrep "${searched}" "${temp}"

rm -- "${temp}"


