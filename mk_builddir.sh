#! /bin/sh

# Copyright (C) 2005-2014 Yusuke Matsunaga
# All rights reserved

# [概要]
#
# build ディレクトリの autogen スクリプトの生成を行うスクリプト
#
# [使用方法]
#
# ./mk_builddir.sh <build-dir>
#
# <build-dir>: 実際にコンパイルを行う作業ディレクトリ


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

# コマンドラインの引数の解析
case $# in
    1) ;;
    *)
	echo "USAGE: $0 <build-dir>"
	exit 1
	;;
esac

builddir=$1

mk_autogen $BASEDIR $builddir

# update スクリプトをコピー
cp etc/update $builddir/update
chmod +x $builddir/update

# autogen.local のテンプレートをコピー
cp etc/autogen.common $builddir/autogen.local

# end of mk_builddir.sh
