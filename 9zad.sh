#!/bin/bash

required_type="$(cat planets.txt | tail -n +2 | sort -rn -k 3 -t ';' | head -n 1 | cut -d ';' -f 2)" 
echo "required type: "${required_type}""
min_distance=-1
name=0
mass=0

while read str; do
	dist=$(echo "${str}" | cut -d ';' -f 3)
	echo "${dist}test"
	if [[ "${dist}" -gt "${min_distance}" ]]; then
#	if [[ $(echo ""${min_distance}" < "${dist}""| bc -l) ]]; then
		min_distance="${dist}"
		name="$(echo "${str}" | cut -d ';' -f 1)"
		mass="$(echo "${str}" | cut -d ';' -f 4)"
	fi
done < <(cat planets.txt| tail -n +2 | grep "${required_type}")
echo "${name}"	"${mass}"
