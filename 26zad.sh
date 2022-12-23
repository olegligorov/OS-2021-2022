#!/bin/bash

if [ $(id -u) -ne 0 ]; then
	echo "The script isn't ran by the root user"
	exit 0
fi

while read user; do
	TOTAL_RSS=0
	PS_CNT=0
	MAX_RSS=-1
	MAX_PID_RSS=-1

	while read upid urss; do
		PS_CNT=$(( PS_CNT + 1 ))
		TOTAL_RSS=$(( TOTAL_RSS + urss ))

		MAX_RSS="${urss}"
		MAX_PID_RSS="${upid}"
	done < <(ps -u "${user}" -o pid=,rss= | sort -n -k 2)
	
	if [ "${PS_CNT}" -eq 0]; then
		continue
	fi

	AVG_RSS=$(( TOTAL_RSS / PS_CNT ))
	echo "${user} ${PS_CNT} ${TOTAL_RSS}"
	if [[ "${MAX_RSS}" -gt $(( AVG_RSS * 2 )) ]]; then
		echo "Terminating process "${MAX_PID_RSS}""
		kill -s TERM "${MAX_PID_RSS}"
		sleep 2
		kill -s KILL "${MAX_PID_RSS}"
	fi

done < <(ps -e -o user= | sort | uniq)
