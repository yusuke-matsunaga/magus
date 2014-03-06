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

noinst_LTLIBRARIES = libatpg_minpat.la


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
	-I$(top_srcdir)/src


############################################################
## libatpg_minpat.la 用の変数の定義
############################################################

libatpg_minpat_la_SOURCES = \
	MinPatImpl.h \
	MinPatImpl.cc \
	Fop2MinPat.h \
	Fop2MinPat.cc \
	Fop2Rofsim.h \
	Fop2Rofsim.cc \
	Verifier.h \
	Verifier.cc

libatpg_minpat_la_LIBADD =

libatpg_minpat_la_LDFLAGS =
