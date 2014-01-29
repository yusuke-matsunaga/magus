## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	test_lex.cc \
	test_parser.h \
	test_parser.cc \
	test_parser.output \
	y.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	matrix_test \
	maxclique_test \
	mincov


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)


############################################################
## 共通の変数の定義
############################################################

LIBYM_MINCOV = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_mincov/libym_mincov.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## matrix_test 用の変数の定義
############################################################

matrix_test_SOURCES = \
	TestMatrix.h \
	TestMatrix.cc \
	test_parser.yy \
	test_lex.ll

matrix_test_LDADD = $(LIBYM_MINCOV)

matrix_test_LDFLAGS =


############################################################
## maxclique_test 用の変数の定義
############################################################

maxclique_test_SOURCES = \
	maxclique_test.cc

maxclique_test_LDADD = $(LIBYM_MINCOV)


############################################################
## mincov 用の変数の定義
############################################################

mincov_SOURCES = \
	mincov.cc

mincov_LDADD = $(LIBYM_MINCOV)

mincov_LDFLAGS =
