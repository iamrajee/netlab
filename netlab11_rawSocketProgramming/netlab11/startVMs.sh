#!/bin/bash
#Script to start the VMs

VMs="r1 r2 h1 h2"
echo "Starting the VMs..."
for i in $VMs; do
	VBoxManage startvm "$i" --type headless
	sleep 5
done	
