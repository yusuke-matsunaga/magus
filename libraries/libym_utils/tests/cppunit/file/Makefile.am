## Process this file with automake to produce Makefile.in
##
## Copyright (C) 2014 Yusuke Matsunaga
## All rights reserved.


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


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

noinst_LTLIBRARIES = \
	libfile_cppunit.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I $(top_srcdir)/include \
	-I $(top_builddir)/include


############################################################
## libfile_cppunit.la 用の変数の定義
############################################################

libfile_cppunit_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libfile_cppunit_la_SOURCES =

libfile_cppunit_la_LIBADD =

libfile_cppunit_la_LDFLAGS = \
	$(AM_LDFLAGS)
