#!/bin/bash

temp=0
if [[ $(id -u -n) =~ "oracle" ]]; then
	temp=1
fi

if [[ $(id -u -n) =~ "grid" ]]; then
	temp=1
fi

if [[ "${temp}" -eq 0 ]]; then
	echo "Script is not ran by user oracle or grid"
	exit 1
fi


