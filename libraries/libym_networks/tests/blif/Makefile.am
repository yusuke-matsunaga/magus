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
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/blif


############################################################
## 共通の変数の定義
############################################################

LIBYM_BLIF = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## parser_test 用の変数の定義
############################################################

parser_test_SOURCES = \
	parser_test.cc \
	TestBlifHandler.h \
	TestBlifHandler.cc \
	NullBlifHandler.h \
	NullBlifHandler.cc

parser_test_LDADD = $(LIBYM_BLIF)

parser_test_LDFLAGS =


############################################################
## lexp_test 用の変数の定義
############################################################

lexptest_SOURCES = \
	lexptest.cc

lexptest_LDADD = $(LIBYM_BLIF)

lexptest_LDFLAGS =


############################################################
## reader_test 用の変数の定義
############################################################

reader_test_SOURCES = \
	reader_test.cc

reader_test_LDADD = $(LIBYM_BLIF)

reader_test_LDFLAGS =
