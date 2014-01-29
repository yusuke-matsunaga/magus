## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ym_mincov \
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

noinst_LTLIBRARIES = libym_mincov.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_mincov.la 用の変数の定義
############################################################

libym_mincov_la_SOURCES = \
	MincovCost.cc \
	MincovMatrix.cc \
	MaxClique.h \
	MaxClique.cc \
	MincovSolver.cc

libym_mincov_la_LIBADD =

libym_mincov_la_LDFLAGS =
