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

noinst_PROGRAMS = \
	gctest


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
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
	GcNode.h \
	GcMgr.h \
	GcMgr.cc

libatpg_minpat_la_LIBADD =

libatpg_minpat_la_LDFLAGS =


############################################################
## gctest 用の変数の定義
############################################################

gctest_SOURCES = \
	gctest.cc

gctest_LDADD = \
	libatpg_minpat.la

gctest_LDFLAGS =
