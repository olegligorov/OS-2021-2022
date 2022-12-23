#!/bin/bash

while read str; do
	name="$(echo "${str}" | cut -d ':' -f 5 | cut -d ',' -f 1 )"
	rest="$(echo "${str}" | cut -d ':' -f 6)"
	echo "${name}:""${rest}"
done < <(cat test_passwd | grep 'Inf' | sort -k 1)
