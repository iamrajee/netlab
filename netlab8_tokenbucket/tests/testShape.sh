#!/bin/sh
#!/bin/bash
echo "Testing Token Bucket......"
for C in 0 500 1000
do
	for R in 1 10
	do	
		rm -rf tempOut
		timeout 3s ./shape "$C" "$R" < arrivals.txt > tempOut
		out=`echo shape_"$C"_"$R".txt`
		val=`diff -B -U 0 tempOut "$out" | grep ^@ | wc -l`
		if [ $val -eq 0 ]
		then
			echo "Capacity $C, rate $R: Passed"
		else
			echo "Capacity $C, rate $R: Failed"
		fi
	done
done
rm -rf tempOut
