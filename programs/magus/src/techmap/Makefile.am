## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	cellmap \
	lutmap \
	lutmapx \
	. \
	tests


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

noinst_LTLIBRARIES = libtechmap.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libtechmap.la 用の変数の定義
############################################################

libtechmap_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libtechmap_la_SOURCES = dummy.cc

libtechmap_la_LIBADD = \
	sbjgraph/libtechmap_sbjgraph.la \
	cellmap/libtechmap_cellmap.la \
	lutmap/libtechmap_lutmap.la

libtechmap_la_LDFLAGS =
