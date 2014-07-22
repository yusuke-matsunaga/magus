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

noinst_LTLIBRARIES = libtechmap_sbjgraph.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/techmap/include


############################################################
## libtechmap_sbjgraph.la 用の変数の定義
############################################################

libtechmap_sbjgraph_la_SOURCES = \
	SbjDumper.cc \
	SbjGraph.cc \
	SbjMinDepth.h \
	SbjMinDepth.cc \
	SmdNode.h

libtechmap_sbjgraph_la_LIBADD =

libtechmap_sbjgraph_la_LDFLAGS =
