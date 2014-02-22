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
	makenode_test \
	read_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_srcdir)/../libym_verilog/include \
	-I$(top_builddir)/../libym_utils/include \
	@POPT_INCLUDES@


############################################################
## makenode_test 用の変数の定義
############################################################

makenode_test_SOURCES = \
	makenode_test.cc

makenode_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

makenode_test_LDFLAGS =


############################################################
## read_test 用の変数の定義
############################################################

read_test_SOURCES = \
	read_test.cc

read_test_LDADD = \
	$(top_builddir)/src/libym_networks.la

read_test_LDFLAGS = \
	@POPT_LIBS@
