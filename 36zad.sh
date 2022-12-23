#!/bin/bash

file="${1}"
dir="${2}"

[ -d "${dir}" ] || { echo "Directory does not exist" ; exit 1; }
[ -r "${dir}" ] || { echo "Directory is not readable" ; exit 1; }

touch "${file}"
echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "${file}"

while read filename; do
	hostname=$(echo "${filename}" | cut -d '.' -f 1 | rev | cut -d '/' -f 1 | rev )
	phy=$(cat "${filename}" | grep 'Physical Interfaces' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	vlans=$(cat "${filename}" | grep 'VLANs' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	hosts=$(cat "${filename}" | grep 'Hosts' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	failover=$(cat "${filename}" | grep 'Failover' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	vpn3des=$(cat "${filename}" | grep 'VPN-3DES-AES' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	peers=$(cat "${filename}" | grep 'Peers' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	ports=$(cat "${filename}" | grep 'Ports' | cut -d ':' -f 2 | cut -d ' ' -f 2-)
	license=$(cat "${filename}" | grep 'license' | sed "s/This platform has an//" | sed 's/ license.//' | cut -d ' ' -f 2-)
	serial=$(cat "${filename}" | grep 'Serial Number' | cut -d ':' -f 2- | cut -d ' ' -f 2-)
	key=$(cat "${filename}" | grep 'Running Activation Key' | cut -d ':' -f 2- | cut -d ' ' -f 2-)
	echo "${hostname},${phy},${vlans},${hosts},${failover},${vpn3des},${peers},${ports},${license},${serial},${key}" >> "${file}"
done < <(find "${dir}" | grep '.log' )
