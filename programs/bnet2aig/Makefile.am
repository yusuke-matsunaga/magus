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

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	bnet2aig


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include


############################################################
## bnet2aig 用の変数の定義
############################################################

bnet2aig_SOURCES = \
	bnet2aig.cc

bnet2aig_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la

bnet2aig_LDFLAGS =
