#! /bin/sh
#
# mk_builddir.sh
#
# コンパイル用のディレクトリを作るシェルスクリプト
#
# Copyright (C) 2015 Yusuke Matsunaga (松永 裕介)
# All rights reserved

# コマンドの定義
# 環境変数 SED をセットしておけばその値が使われる．
# そうでなければデフォルトの sed が使われる．
echo ${SED:="sed"} > /dev/null

# このファイルのあるディレクトリを srcdir にセットする．
basedir=`dirname $0`
srcdir=`cd $basedir; pwd`

# オプション解析用のループ
#while [ $# -ge 4 ] ; do
#    case $1 in
#	"--ymtools_dir")
#	    ymtoolsdir=$2
#	    shift; shift;;
#
#	"--ymtclpp_dir")
#	    ymtclppdir=$2
#	    shift; shift;;
#
#	*) ;;
#    esac
#done

# 引数の数が異なっていたら usage を表示して終わる．
if [ $# -ne 2 ]; then
    echo "USAGE mk_builddir.sh <compiledir> <installdir>"
    exit 1
fi

# ビルド用のディレクトリ名
builddir=$1

# インストール先のディレクトリ名
installdir=$2

# ディレクトリ名を表示して確認を求める．
echo "****"
echo "source  directory: $srcdir"
echo "build   directory: $builddir"
echo "install directory: $installdir"
echo "****"
echo -n "continue with above configuration ? (yes/no): "
while read confirmation; do
    case $confirmation in
	"yes")
	    break;;
	"no")
	    echo "Setup canceled."
	    exit 0;;
	*)
	    echo "please answer 'yes' or 'no'"
	    echo -n "continue ? (yes/no)"
	    ;;
    esac
done

# ビルドディレクトリはなければ作る．
test -d $builddir || mkdir -p $builddir

# boot.sh ファイルを作る．
boot="boot.sh"
$SED -e s!___SRC_DIR___!$srcdir! \
     -e s!___INSTALL_DIR___!$installdir! \
     $srcdir/etc/${boot}.in > $builddir/$boot
chmod +x $builddir/$boot

echo "Build directory setup completed."
echo "Move to '$builddir', and execute './$boot'"
