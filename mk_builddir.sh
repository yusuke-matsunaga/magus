#! /bin/sh

# mk_builddir.sh
#
# コンパイル用のディレクトリを作るシェルスクリプト
#
# Copyright (C) 2015 Yusuke Matsunaga
# All rights reserved


# このファイルのあるディレクトリを srcdir にセットする．
basedir=`dirname $0`
srcdir=`cd $basedir; pwd`

if test "x$1" = "x"; then
    echo "USAGE mk_builddir.sh <compiledir>"
    exit 1
fi

# ビルド用のディレクトリ名
builddir=$1

echo "Making compile directory $builddir for source tree $srcdir ..."

# なければ作る．
test -d $builddir || mkdir -p $builddir

# その下にサブディレクトリを作る．
for subdir in release debug profile; do
    path="$builddir/$subdir"
    test -d $path || mkdir -p $path
done

# mk ファイルを作る．
sed -e s!__YM_SRC_DIRECTORY__!$srcdir! \
    $srcdir/etc/mk.in > $builddir/mk
chmod +x $builddir/mk

# mk boot を実行する．
(cd $builddir && ./mk boot)

echo "  done"
