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

ymtoolsdir=NONE
ymtclppdir=NONE
while [ $# -ge 4 ] ; do
    case $1 in
	"--ymtools_dir")
	    ymtoolsdir=$2
	    shift; shift;;

	"--ymtclpp_dir")
	    ymtclppdir=$2
	    shift; shift;;

	*) ;;
    esac
done

if [ $# -ne 2 ]; then
    echo "USAGE mk_builddir.sh [--ymtools_dir <ymtools-dir>] [--ymtclpp_dir <ymtclpp-dir>] <compiledir> <installdir>"
    exit 1
fi

# ビルド用のディレクトリ名
builddir=$1

# インストール先のディレクトリ名
installdir=$2

if [ $ymtoolsdir = NONE ]; then
    ymtoolsdir=$installdir
fi

if [ $ymtclppdir = NONE ]; then
    ymtclppdir=$installdir
fi

echo "****"
echo "source  directory: $srcdir"
echo "build   directory: $builddir"
echo "install directory: $installdir"
echo "ymtools directory: $ymtoolsdir"
echo "ymtclpp directory: $ymtclppdir"
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
    -e s!__YM_YMTOOLS_DIRECTORY__!$ymtoolsdir! \
    -e s!__YM_YMTCLPP_DIRECTORY__!$ymtclppdir! \
    $srcdir/etc/${do_cmake}.in > $builddir/${do_cmake}
chmod +x $builddir/${do_cmake}

# do_cmake.sh を実行する．
(cd $builddir && ./${do_cmake})

echo "  done"
