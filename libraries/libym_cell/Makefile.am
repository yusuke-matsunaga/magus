## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_cell \
	ci \
	mislib \
	dotlib \
	libcomp \
	$(YM_EXTRA_SUBDIRS) \
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

noinst_LTLIBRARIES = libym_cell.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_cell.la 用の変数の指定
############################################################

libym_cell_la_SOURCES = \
	CellFFInfo.cc \
	CellLatchInfo.cc \
	display.cc

libym_cell_la_LIBADD = \
	ci/libym_cell_ci.la \
	dotlib/libym_cell_dotlib.la \
	mislib/libym_cell_mislib.la \
	libcomp/libym_cell_libcomp.la

libym_cell_la_LDFLAGS =
