#! /bin/sh

# Copyright (C) 2005-2012 Yusuke Matsunaga
# All rights reserved

# [概要]
#
# build ディレクトリの autogen スクリプトの生成を行うスクリプト
#
# [使用方法]
#
# ./mkautogen <root-dir> <root-name>
#
# <root-dir>: build directory と install directory の元となる directory
#             を指定する．
#             build directory は   <root-dir>/build/<root-name>
#             install directory は <root-dir>/test/<root-name>
#             となる．
#             実際にはその直下に
#             opt:   最適化オプション付きでコンパイルしたもの
#             debug: デバッグオプション付きでコンパイルしたもの
#             prof:  プロファイリングオプション付きでコンパイルしたもの
#             という3つのサブディレクトリができる．


# function を使えるシェルかどうかのチェック
# GNU Autoconf/Automake/Libtool 21.1.4 のコードを元にしたけど
# 本に載っているコードはそのままでは引数を渡すことができない．
# また SHELL という環境変数がすでに定義済だとうまく動かない．

# zsh の場合
if test -n "$ZSH_VERSION"; then
  emulate sh
  NULLCMD=:
fi

# bash の場合
test -n "$BASH_VERSION" && set -o posix

TMP_SHELL="${TMP_SHELL-/bin/sh}"
if test x"$1" = x--re-executed; then
  shift
elif "$TMP_SHELL" -c 'foo () { exit 0; }; foo' 2>/dev/null; then
  :
else
  for cmd in sh bash ash bsh ksh zsh sh5; do
    args=${1+"$@"}
    set `IFS=":"; X="$PATH:/bin:/usr/bin:/usr/afsws/bin:/usr/ucb"; echo $X`
    for dir
    do
      shell="$dir/$cmd"
      if (test -f "$shell" || test -f "$shell.exe") &&
        "$shell" -c 'foo () { exit 0; }; foo' 2>/dev/null
      then
        SHELL="$shell" exec "$shell" "$0" --re-executed $args
      fi
    done
  done
  echo "Unable to locate a shell interpreter with function support" >&2
  exit 1
fi

# このファイルのあるディレクトリを BASEDIR にセットする．
basedir=`dirname $0`
BASEDIR=`cd $basedir; pwd`

# 共通な関数を読み込む
. $BASEDIR/etc/mk.subr

# プログラム名の設定
set_program


case $# in
  2) ;;
  *)
     echo "USAGE: mkautogen rootdir basename"
     exit 255
     ;;
esac

ROOTDIR=$1
ROOTNAME=$2
builddir="$ROOTDIR/build/$ROOTNAME"
testdir="$ROOTDIR/test/$ROOTNAME"

MODES="opt debug prof"
for mode in $MODES
do
    mk_autogen $BASEDIR $builddir/$mode $testdir/$mode $mode
done

# 根元の Makefile の生成
mkfile="$builddir/Makefile"
cat <<EOF > $mkfile
all:
	cd opt; make all
	cd debug; make all
	cd prof; make all

install:
	cd opt; make install
	cd debug; make install
	cd prof; make install

clean:
	cd opt; make clean
	cd debug; make clean
	cd prof; make clean

configure:
	cd opt; ./autogen
	cd debug; ./autogen
	cd prof; ./autogen

EOF

# update スクリプトのコピー
cp etc/update $builddir/update
cp etc/autogen.common $builddir/autogen.common

# end of mkautogen