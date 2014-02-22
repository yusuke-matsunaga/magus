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

bin_PROGRAMS = \
	base_test \
	bliftest \
	change_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logics/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## base_test 用の変数の定義
############################################################

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

base_test_LDFLAGS =


############################################################
## bliftest 用の変数の定義
############################################################

bliftest_SOURCES = \
	bliftest.cc

bliftest_LDADD = \
	$(top_builddir)/src/libym_networks.la

bliftest_LDFLAGS =


############################################################
## change_test 用の変数の定義
############################################################

change_test_SOURCES = \
	change_test.cc

change_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

change_test_LDFLAGS =
