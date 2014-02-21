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
    2) ;;
    *)
	echo "USAGE: $0 <build-dir> <prefix-dir>"
	exit 1
	;;
esac

build_dir=$1
prefix_dir=$2

mk_autogen $BASEDIR $build_dir

# update スクリプトをコピー
cp etc/update $build_dir/update
chmod +x $build_dir/update

# autogen.local を生成
sed -e s!__YM_PREFIX_DIR__!$prefix_dir! \
    etc/autogen.local.tmpl > $build_dir/autogen.local


# py-sip の configure.py スクリプトを生成する．
for dir in utils; do
    mk_configure_py $BASEDIR/py-sip/$dir $build_dir/py-sip/$dir $prefix_dir
done

# end of mk_builddir.sh
