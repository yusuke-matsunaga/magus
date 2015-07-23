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

case $# in
    2)
    ;;

    *)
	echo "USAGE mk_builddir.sh <compiledir> <installdir>"
	exit 1;;
esac

# ビルド用のディレクトリ名
builddir=$1

# インストール先のディレクトリ名
installdir=$2

echo "****"
echo "source  directory: $srcdir"
echo "build   directory: $builddir"
echo "install directory: $installdir"
echo "****"
echo -n "continue ? (yes/no)"
while read confirmation; do
    case $confirmation in
	"yes")
	    break;;
	"no")
	    exit 0;;
	*)
	    echo "please answer 'yes' or 'no'"
	    echo -n "continue ? (yes/no)"
	    ;;
    esac
done

# ビルドディレクトリはなければ作る．
test -d $builddir || mkdir -p $builddir

# do_cmake ファイルを作る．
do_cmake="do_cmake.sh"
sed -e s!__YM_SRC_DIRECTORY__!$srcdir! \
    -e s!__YM_INSTALL_DIRECTORY__!$installdir! \
    $srcdir/etc/${do_cmake}.in > $builddir/${do_cmake}
chmod +x $builddir/${do_cmake}

# do_cmake.sh を実行する．
(cd $builddir && ./${do_cmake})

echo "  done"
