#!/bin/sh
#!/bin/bash
echo "Testing FIFO......"
for R in 0.1 1.0 10.0 100.0
do
	rm -rf tempOut
	timeout 3s ./rr "$R" < arrivals.txt > tempOut
	out=`echo rr_"$R"`
	val=`diff -B -U 0 tempOut "$out" | grep ^@ | wc -l`
	if [ $val -eq 0 ]
	then
		echo "Rate $R: Passed"
	else
		echo "Rate $R: Failed"
	fi
done
rm -rf tempOut
