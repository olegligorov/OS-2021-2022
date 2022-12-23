#!/bin/bash

while read str; do
	chmod 0664 "${str}"
done < <(find ~/ -perm '0644')
