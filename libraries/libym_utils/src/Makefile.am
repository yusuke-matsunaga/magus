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

lib_LTLIBRARIES = libym_utils.la


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
