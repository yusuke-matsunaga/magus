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
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## 共通の変数の定義
############################################################

LIBYM_NET = \
	../../libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## base_test 用の変数の定義
############################################################

base_test_SOURCES = \
	base_test.cc

base_test_LDADD = \
	$(LIBYM_NET)

base_test_LDFLAGS =


############################################################
## bliftest 用の変数の定義
############################################################

bliftest_SOURCES = \
	bliftest.cc

bliftest_LDADD = \
	$(LIBYM_NET)

bliftest_LDFLAGS =


############################################################
## change_test 用の変数の定義
############################################################

change_test_SOURCES = \
	change_test.cc

change_test_LDADD = \
	$(LIBYM_NET)

change_test_LDFLAGS =
