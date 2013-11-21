#! /bin/sh

IGF=./opt/programs/igf/src/igf/igf
TLIMIT=2
BASEDIR=~/var/sandbox/ymtools-igf/programs/igf/data

run() {
    n=$1
    k=$2
    m=$3
    x=$4
    dir=${BASEDIR}/${n}-${k}
    for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20; do
	filename=$dir/data_${n}_${k}_${i}
	echo "$IGF -m $m -x $x -t $TLIMIT $filename"
	$IGF -m $m -x $x -t $TLIMIT $filename
    done
}

run 20 127 1 1
run 20 511 1 1
run 20 1023 1 1
run 20 2047 1 1

run 20 127 1 2
run 20 511 1 2
run 20 1023 1 2
run 20 2047 1 2

run 20 127 1 3
run 20 511 1 3
run 20 1023 1 3
run 20 2047 1 3

run 20 127 1 4
run 20 511 1 4
run 20 1023 1 4
run 20 2047 1 4

run 20 127 2 1
run 20 511 2 1
run 20 1023 2 1
run 20 2047 2 1

run 20 127 2 2
run 20 511 2 2
run 20 1023 2 2
run 20 2047 2 2

run 20 127 2 3
run 20 511 2 3
run 20 1023 2 3
run 20 2047 2 3

run 20 127 2 4
run 20 511 2 4
run 20 1023 2 4
run 20 2047 2 4

run 20 127 3 1
run 20 511 3 1
run 20 1023 3 1
run 20 2047 3 1

run 20 127 3 2
run 20 511 3 2
run 20 1023 3 2
run 20 2047 3 2

run 20 127 3 3
run 20 511 3 3
run 20 1023 3 3
run 20 2047 3 3

run 20 127 3 4
run 20 511 3 4
run 20 1023 3 4
run 20 2047 3 4

run 20 127 4 1
run 20 511 4 1
run 20 1023 4 1
run 20 2047 4 1

run 20 127 4 2
run 20 511 4 2
run 20 1023 4 2
run 20 2047 4 2

run 20 127 4 3
run 20 511 4 3
run 20 1023 4 3
run 20 2047 4 3

run 20 127 4 4
run 20 511 4 4
run 20 1023 4 4
run 20 2047 4 4

run 20 127 5 1
run 20 511 5 1
run 20 1023 5 1
run 20 2047 5 1

run 20 127 5 2
run 20 511 5 2
run 20 1023 5 2
run 20 2047 5 2

run 20 127 5 3
run 20 511 5 3
run 20 1023 5 3
run 20 2047 5 3

run 20 127 5 4
run 20 511 5 4
run 20 1023 5 4
run 20 2047 5 4

run 20 127 6 1
run 20 511 6 1
run 20 1023 6 1
run 20 2047 6 1

run 20 127 6 2
run 20 511 6 2
run 20 1023 6 2
run 20 2047 6 2

run 20 127 6 3
run 20 511 6 3
run 20 1023 6 3
run 20 2047 6 3

run 20 127 6 4
run 20 511 6 4
run 20 1023 6 4
run 20 2047 6 4
