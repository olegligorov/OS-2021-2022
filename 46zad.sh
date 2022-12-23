#!/bin/bash

if [ $# -ne 3 ]; then
	echo "The script needs 3 arguments"
	exit 1
fi

num="${1}"
prefix="${2}"
unit="${3}"

decimal=$(cat "prefix.csv" | grep ",${prefix}," | cut -d ',' -f 3)
unit_symbol=$(cat "base.csv" | grep ",${unit}," | cut -d ',' -f 3,1)

number=$(echo "${num} * ${decimal}" | bc)
echo "${number} ${unit} (${unit_symbol})"
