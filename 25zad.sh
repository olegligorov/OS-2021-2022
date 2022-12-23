#!/bin/bash

if [ $# - ne 3 ]; then
	echo "Invalid arguments"
 	exit 1
fi

SRC="${1}"
DST="${2}"
ABC="${3}"

[ -d "${SRC}" ] || { echo "Source is not a directory"; exit 2; }
[ -d "${DST}" ] || { echo "Destination is not a directory"; exit 2; }

dst_content=$(find "${DST}" -type f | wc -l)

if [ "${dst_content}" -gt 0 ]; then
	echo "Destination is not empty"
	exit 2
fi

if [ $(id -u) -ne 0 ]; then
	echo "Script not run as root"
	exit 0
fi

while read file; do

	real_dir_name="$(echo "${file}" | sed -E "s/"${src}"\///")"

	mkdir -p "${DST}/"$(dirname "${real_dir_name}")"

	mv "${file}" "${DST}"/"${real_dir_name}"

done < <(find "${SRC}" -type f -name "*${ABC}*")
