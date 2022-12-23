#!/bin/bash

while read file; do
	temp=$(mktemp)	
	while read line; do

		type=$(echo "${line}" | cut -d ' ' -f 4)

		if [[ $(echo "${line}" | grep 'serial' | wc -l) -ne 0 ]]; then
			continue
		fi

		if [[ "${type}" != "SOA" ]]; then
			echo "${line}" >> "${temp}"
			continue
		fi	

		if [[ "${type}" == "SOA" ]]; then

			if [[ $(echo "${line}" | grep '(' | wc -l) -eq 0 ]]; then
				before=$(echo "${line}" | rev | cut -d '.' -f 2- | rev)
		   		before="${before}. "	
				echo "before: ""${before}"
				after=$(echo "${line}" | rev | cut -d '.' -f 1 | rev | cut -d ' ' -f 3-)
				after=" ${after}"

				serial=$(echo "${line}" | rev | cut -d '.' -f 1 | rev | cut -d ' ' -f 2)
				currtime=$(printf '%(%Y%m%d)T\n')
			
				serialtime=$(echo "${serial}" | cut -c -8)
			
				if [[ $currtime -gt $serialtime ]]; then
					serial="${currtime}00"
				elif [[ $currtime -eq $serialtime ]]; then
					serial=$(( serial + 1 ))
				fi	
			
				echo "${before}""${serial}""${after}" >> "${temp}"

			elif [[ $(echo "${line}" | grep '(' | wc -l) -eq 1 ]]; then

				serial=$(cat "${file}" | grep -A 1 "${line}" | grep "serial" | cut -d ';' -f 1 | tr -s ' ' | sed 's/[ \t]//')
				currtime=$(printf '%(%Y%m%d)T\n')
				
				serialtime=$(echo "${serial}" | cut -c -8)	
				echo "${serialtime}" "${currtime}"
				if [[ $currtime -gt $serialtime ]]; then
					echo "here"
					serial="${currtime}00"
				elif [[ $currtime -eq $serialtime ]]; then
					serial=$(( serial + 1 ))
				fi 
				echo "${line}" >> "${temp}"
				echo "${serial} ; serial" >> "${temp}"
			fi
		fi

	done < "${file}"
	cat "${temp}" > "${file}"
done < <(echo "${@}")


