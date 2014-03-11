## Process this file with automake to produce Makefile.in
##
## Copyright (C) 2014 Yusuke Matsunaga
## All rights reserved.


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	alloc \
	btg \
	file \
	gcsolver \
	gen \
	io \
	itvl \
	mfset \
	mincov \
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

noinst_PROGRAMS = \
	run_all_test

noinst_LTLIBRARIES = \
	libym_utils_cppunit.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I $(top_srcdir)/include \
	-I $(top_srcdir)/tests/cppunit \
	-I $(top_builddir)/include


############################################################
## run_all_test 用の変数の定義
############################################################

run_all_test_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS) \
	$(CPPUNIT_CFLAGS)

run_all_test_SOURCES = \
	run_all_test.cc

run_all_test_LDADD = \
	libym_utils_cppunit.la

run_all_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libym_utils_cppunit.la 用の変数の定義
############################################################

libym_utils_cppunit_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libym_utils_cppunit_la_SOURCES = \
	YmUtilsTestRegistry.h \
	YmUtilsTestRegistry.cc

nodist_EXTRA_libym_utils_cppunit_la_SOURCES = dummy.cc

libym_utils_cppunit_la_LIBADD = \
	alloc/liballoc_cppunit.la \
	btg/libbtg_cppunit.la \
	file/libfile_cppunit.la \
	gcsolver/libgcsolver_cppunit.la \
	gen/libgen_cppunit.la \
	itvl/libitvl_cppunit.la \
	io/libio_cppunit.la \
	mfset/libmfset_cppunit.la \
	misc/libmisc_cppunit.la \
	msg/libmsg_cppunit.la \
	str/libstr_cppunit.la

libym_utils_cppunit_la_LDFLAGS = \
	$(top_builddir)/src/libym_utils.la \
	$(CPPUNIT_LIBS) \
	$(AM_LDFLAGS)
