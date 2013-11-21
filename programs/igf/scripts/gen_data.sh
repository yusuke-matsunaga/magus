#! /bin/sh

# Copyright (C) 2013 Yusuke Matsunaga
# All rights reserved

# [概要]
# - rvgen を用いて複数のデータを作るスクリプト
#
# [使用方法]
#
# gen_data.sh <N> <K> <base-name> <seed1> <seed2> <seed3> .....
#
# - N:         ベクタの次元
# - K:         ベクタ数
# - base-name: データのファイル名の基本部分
# - seed?:     乱数のシード
#
# 1番めのファイルは <base-name>-N-K-1
# 2番めのファイルは <base-name>-N-K-2
# という風に名前がつけられる．

# rvgen プログラムのパス
RVGEN=./rvgen

if [ $# -lt 4 ]; then
    echo "Usage: $0 <N> <K> <base-name> <seed1> <seed2> ... <seedN>"
    exit 1
fi

N=$1
K=$2
BASENAME=$3

echo "N = $N, K = $K, BASENAME = $BASENAME"

I=1
while [ "x$4" != "x" ]; do
    filename="$BASENAME-$N-$K-$I"
    echo "$RVGEN --seed $4 $N $K > $filename"
    $RVGEN --seed $4 $N $K > $filename
    shift
    I=`expr $I + 1`
done
