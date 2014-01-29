## Process this file with automake to produce Makefile.in


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

noinst_LTLIBRARIES = libmagus_test.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/magus_tclsh/include


############################################################
## libmagus_test.la 用の変数の定義
############################################################

libmagus_test_la_SOURCES = \
	TestCmd.h \
	TestCmd.cc

libmagus_test_la_LIBADD =

libmagus_test_la_LDFLAGS = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la
