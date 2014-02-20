## Process this file with automake to produce Makefile.in
##
## Copyright (C) 2014 Yusuke Matsunaga
## All rights reserved.


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	alloc \
	file \
	gen \
	io \
	itvl \
	misc \
	msg \
	str


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

lib_LTLIBRARIES = \
	libym_utils.la \
	libym_utils_d.la \
	libym_utils_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS =


############################################################
## libym_utils.la 用の変数の定義
############################################################

libym_utils_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_utils_la_SOURCES = dummy.cc

libym_utils_la_LIBADD = \
	alloc/liballoc.la \
	file/libfile.la \
	gen/libgen.la \
	itvl/libitvl.la \
	io/libio.la \
	misc/libmisc.la \
	msg/libmsg.la \
	str/libstr.la

libym_utils_la_LDFLAGS =


############################################################
## libym_utils_d.la 用の変数の定義
############################################################

libym_utils_d_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_utils_d_la_SOURCES = dummy.cc

libym_utils_d_la_LIBADD = \
	alloc/liballoc_d.la \
	file/libfile_d.la \
	gen/libgen_d.la \
	itvl/libitvl_d.la \
	io/libio_d.la \
	misc/libmisc_d.la \
	msg/libmsg_d.la \
	str/libstr_d.la

libym_utils_d_la_LDFLAGS =


############################################################
## libym_utils_p.la 用の変数の定義
############################################################

libym_utils_p_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_utils_p_la_SOURCES = dummy.cc

libym_utils_p_la_LIBADD = \
	alloc/liballoc_p.la \
	file/libfile_p.la \
	gen/libgen_p.la \
	itvl/libitvl_p.la \
	io/libio_p.la \
	misc/libmisc_p.la \
	msg/libmsg_p.la \
	str/libstr_p.la

libym_utils_p_la_LDFLAGS =
