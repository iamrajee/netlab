#!/bin/bash
# Script to connect via SSH to the various devices
declare -A vmMap
vmMap["h1"]=14501
vmMap["h2"]=14502
vmMap["r1"]=14601
vmMap["r2"]=14602

/usr/bin/expect -c 'spawn ssh -p '${vmMap[$1]}' -o StrictHostKeyChecking=no tc@localhost; expect "password:"; send "user@123\n"; interact'
