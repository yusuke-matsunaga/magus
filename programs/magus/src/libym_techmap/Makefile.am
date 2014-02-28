## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_techmap \
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

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_techmap.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_techmap.la 用の変数の定義
############################################################

libym_techmap_la_SOURCES =

libym_techmap_la_LIBADD = \
	cellmap/libym_techmap_cell.la \
	lutmap/libym_techmap_lut.la

libym_techmap_la_LDFLAGS =
