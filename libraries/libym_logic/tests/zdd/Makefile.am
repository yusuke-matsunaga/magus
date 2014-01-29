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

noinst_LTLIBRARIES = libymzddtest.la

noinst_PROGRAMS = \
	base_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES)


############################################################
## 共通の変数の指定
############################################################

YM_BDD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## libym_zdd_test.la 用の変数の指定
############################################################

libymzddtest_la_SOURCES = \
	zddtest.h \
	CombSet.h \
	CombSet.cc \
	test_utils.cc

libymzddtest_la_LIBADD =

libymzddtest_la_LDFLAGS =


############################################################
## base_test 用の変数の指定
############################################################

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	libymzddtest.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la

base_test_LDFLAGS =
