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

noinst_PROGRAMS = \
	parser_test \
	lexptest \
	reader_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/blif \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## parser_test 用の変数の定義
############################################################

parser_test_SOURCES = \
	parser_test.cc \
	TestBlifHandler.h \
	TestBlifHandler.cc \
	NullBlifHandler.h \
	NullBlifHandler.cc

parser_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

parser_test_LDFLAGS =


############################################################
## lexp_test 用の変数の定義
############################################################

lexptest_SOURCES = \
	lexptest.cc

lexptest_LDADD = \
	$(top_builddir)/src/libym_networks.la

lexptest_LDFLAGS =


############################################################
## reader_test 用の変数の定義
############################################################

reader_test_SOURCES = \
	reader_test.cc

reader_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

reader_test_LDFLAGS =
