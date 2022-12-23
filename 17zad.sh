#!/bin/bash

if [ $(id -u) -ne 0 ]; then
	echo "Script is not ran by root"
	exit 1
fi

while read line; do
	user=$(echo $line | cut -d ':' -f 1)
	uid=$(echo $line | cut -d ':' -f 3)
	gid=$(echo $line | cut -d ':' -f 4)
	home=$(echo $line | cut -d ':' -f 6)

	if [ -z $home ]; then
		echo "$user has no homedir set"
		continue
	fi

	if [ ! -d $home ]; then
		echo "$user has homedir $home which is not a directory"
		continue
	fi

	dirperm=$(ls -ld $home | cut -d ' ' -f 1)
	dirowner=$(ls -ld $home | cut -d ' ' -f 3)

	if [ $dirowner != $user ]; then
		echo "$user is not owner of $home"
		continue
	fi
	
	if [ $(echo $dirperm | cut -c 3) != 3 ]; then
		echo "$user can not write in $home"
	fi

done < <(cat /etc/passwd)

