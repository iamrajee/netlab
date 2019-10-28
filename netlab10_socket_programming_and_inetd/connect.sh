#!/bin/bash
# Script to connect via SSH to the various devices
declare -A vmMap
vmMap["h1"]=14501
vmMap["h2"]=14502
vmMap["h3"]=14503

/usr/bin/expect -c 'spawn ssh -p '${vmMap[$1]}' -o StrictHostKeyChecking=no tc@localhost; expect "password:"; send "user@123\n"; interact'
