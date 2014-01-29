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
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@


############################################################
## 共通の変数の定義
############################################################

LIBYM_NET = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


############################################################
## makenode_test 用の変数の定義
############################################################

makenode_test_SOURCES = \
	makenode_test.cc

makenode_test_LDADD = \
	$(LIBYM_NET)

makenode_test_LDFLAGS =


############################################################
## read_test 用の変数の定義
############################################################

read_test_SOURCES = \
	read_test.cc

read_test_LDADD = \
	$(LIBYM_NET)

read_test_LDFLAGS = \
	@POPT_LIBS@
