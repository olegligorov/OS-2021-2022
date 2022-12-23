#!/bin/bash

max=$(cat spacex.txt | grep 'Failure' | cut -d '|' -f 2 | sort | uniq -c | sort -k 1 -rn | head -n 1 | tr -s " " | cut -d ' ' -f 3)
file=$(cat spacex.txt | grep "${max}" | sort -k 1 -rn -t '|' | head -n 1)
outcome=$(cat spacex.txt | grep "${file}" | cut -d '|' -f 3)
payload=$(cat spacex.txt | grep "${file}" | cut -d '|' -f 4)

echo "${outcome}":"${payload}"
