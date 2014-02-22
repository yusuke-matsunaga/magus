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
	parsertest \
	reader_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/iscas89 \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## parsertest 用の変数の定義
############################################################

parsertest_SOURCES = \
	parsertest.cc

parsertest_LDADD = \
	$(top_builddir)/src/libym_networks.la

parsertest_LDFLAGS =


############################################################
## reader_test 用の変数の定義
############################################################

reader_test_SOURCES = \
	reader_test.cc

reader_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

reader_test_LDFLAGS =
