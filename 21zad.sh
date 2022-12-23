#!/bin/bash

[ $# -eq 3 ] || { echo "Invalid numbers of arguments"; exit 1; }

file="${1}"
key1="${2}"
key2="${3}"

val1=$(cat "${file}" | grep "${key1}" | cut -d '=' -f 2 | tr ' ' '\n' | sort | uniq )
val2=$(grep -vxF -f <(echo "${val1}") <(grep "${key2}" "${file}" | cut -d '=' -f 2 | awk '${1=$1}1' | tr ' ' '\n' | sort | uniq))
#awk '{$1=$1}1' posle cut
newval1=$(echo "${val1}" | tr '\n' ' ')
newval2=$(echo "${val2}" | tr '\n' ' ')
																		
sed -i -e "s/^${key1}=.*/${key1}=${newval1}/; s/^${key2}=.*/${key2}=${newval2}/" "${file}"

#i would have done it
#sed -i -e 's/${val2}/${newval2}/g' "${file}"
