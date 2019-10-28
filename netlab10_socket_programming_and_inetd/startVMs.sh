#!/bin/bash
#Script to start the VMs

VMs="h1 h2 h3"
echo "Starting the VMs..."
for i in $VMs; do
	VBoxManage startvm "$i" --type headless
	sleep 10
done	
