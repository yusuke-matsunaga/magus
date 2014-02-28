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

lib_LTLIBRARIES = \
	libym_popt.la \
	libym_popt_d.la \
	libym_popt_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	@POPT_INCLUDES@


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = PoptMainApp.cc


############################################################
## libym_popt.la 用の変数の定義
############################################################

libym_popt_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libym_popt_la_SOURCES = \
	$(COMMON_SOURCES)

libym_popt_la_LIBADD =

libym_popt_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	@POPT_LIBS@


############################################################
## libym_popt_d.la 用の変数の定義
############################################################

libym_popt_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libym_popt_d_la_SOURCES = \
	$(COMMON_SOURCES)

libym_popt_d_la_LIBADD =

libym_popt_d_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	@POPT_LIBS@


############################################################
## libym_popt_p.la 用の変数の定義
############################################################

libym_popt_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libym_popt_p_la_SOURCES = \
	$(COMMON_SOURCES)

libym_popt_p_la_LIBADD =

libym_popt_p_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	@POPT_LIBS@
