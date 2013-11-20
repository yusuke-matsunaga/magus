#! /bin/sh

RVGEN=./opt/programs/igf/src/rvgen/rvgen

gen_data() {
    n=$1
    k=$2
    echo "n = $n, k = $k"
    for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
	filename="data_${n}_${k}_$i"
	echo "filename = $filename"
	if [ -f $filename ]; then
	    rm -f $filename
	fi
	$RVGEN -s $i $n $k > $filename
    done
}

gen_data 20 7
gen_data 20 127
gen_data 20 255
gen_data 20 511
gen_data 20 1023
gen_data 20 2047
gen_data 40 7
gen_data 40 127
gen_data 40 255
gen_data 40 511
gen_data 40 1023
gen_data 40 2047
