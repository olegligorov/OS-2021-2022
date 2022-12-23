#!/bin/bash

SRC="${1}"
DIR="${2}"

if [ $# -ne 2 ]; then
	echo "There must be exactly 2 arguments"
	exit 1
fi

if [ ! -d "${SRC}" ]; then
	echo "Directory "${SRC}" does not exist"
	exit 1
fi

mkdir -p "${DIR}"

album="misc"
mkdir -p "${DIR}/images"
mkdir -p "${DIR}/by-date"


while read str; do

	title=$(echo "${str}" | cut -d '.' -f 1 | sed 's/([^)]*)//g' | tr -s ' ')
	if [[ $(echo "${str}" | grep '(' | wc -l) -gt 0 ]]; then
		album=$(echo "${str}" | rev | cut -d '(' -f 1 | cut -d ')' -f 2 | rev | tr -s ' ')
	else
		album="misc"
	fi
	date=$(stat "${str}" --printf "%y\n" | cut -d ' ' -f 1)
	hash=$(sha256sum "${str}" | cut -c -16)
	
	dest="${DIR}/images/${str}"
	cp "${str}" "${dest}"
	
	if [ ! -d "${dir}/by-date/${date}/by-album/${album}/by-title/" ]; then
		mkdir -p "${dir}/by-date/${date}/by-album/${album}/by-title/"

	fi
	
	ln -s "${dest}" "${dir}/by-date/${date}/by-album/${album}/by-title/${title}.jpg"
	
	if [ ! -d "${dir}/by-date/${date}/by-title/" ]; then
		mkdir -p "${dir}/by-date/${date}/by-title/"
	fi
	ln -s "${dest}" "${dir}/by-date/${date}/by-title/${title}.jpg"

	if [ ! -d "${dir}/by-album/${album}/by-date/${date}/by-title/" ]; then
		mkdir -p "${dir}/by-album/${album}/by-date/${date}/by-title/"
	fi
	ln -s "${dest}" "${dir}/by-album/${album}/by-date/${date}/by-title/${title}.jpg"
	
	if [ ! -d "${dir}/by-album/${album}/by-title/" ]; then
		mkdir -p "${dir}/by-album/${album}/by-title/"
	fi
	ln -s "${dest}" "${dir}/by-album/${album}/by-title/${title}.jpg"
	
	if [ ! -d "${dir}/by-title/" ]; then
		mkdir -p "${dir}/by-title/"
	fi
	ln -s "${dest}" "${dir}/by-title/${title}.jpg"

done < <(find "${SRC}" -type f -printf '%f' | grep '.jpg')

