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
	lmtest \
	bliftest \
	iscas89test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/tgnet \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## lmtest 用の変数の定義
############################################################

lmtest_SOURCES = \
	lmtest.cc

lmtest_LDADD = \
	$(top_builddir)/src/libym_networks.la

lmtest_LDFLAGS =


############################################################
## bliftest 用の変数の定義
############################################################

bliftest_SOURCES = \
	bliftest.cc

bliftest_LDADD = \
	$(top_builddir)/src/libym_networks.la

bliftest_LDFLAGS =


############################################################
## iscas89test 用の変数の定義
############################################################

iscas89test_SOURCES = \
	iscas89test.cc

iscas89test_LDADD = \
	$(top_builddir)/src/libym_networks.la

iscas89test_LDFLAGS =
