## Process this file with automake to produce Makefile.in
##
## Copyright (C) 2014 Yusuke Matsunaga
## All rights reserved.


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

noinst_LTLIBRARIES = \
	libgcsolver.la \
	libgcsolver_p.la \
	libgcsolver_d.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	GcNode.h \
	GcSolver.cc


############################################################
## libgcsolver.la 用の変数の定義
############################################################

libgcsolver_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libgcsolver_la_SOURCES = $(COMMON_SOURCES)

libgcsolver_la_LIBADD = \
	$(AM_LIBADD)

libgcsolver_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libgcsolver.la 用の変数の定義
############################################################

libgcsolver_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libgcsolver_d_la_SOURCES = $(COMMON_SOURCES)

libgcsolver_d_la_LIBADD = \
	$(AM_LIBADD)

libgcsolver_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libgcsolver_p.la 用の変数の定義
############################################################

libgcsolver_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libgcsolver_p_la_SOURCES = $(COMMON_SOURCES)

libgcsolver_p_la_LIBADD = \
	$(AM_LIBADD)

libgcsolver_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
