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
	mvnconv_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_srcdir)/../libym_verilog/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## mvnconv_test 用の変数の定義
############################################################

mvnconv_test_SOURCES = \
	mvnconv_test.cc

mvnconv_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

mvnconv_test_LDFLAGS =
