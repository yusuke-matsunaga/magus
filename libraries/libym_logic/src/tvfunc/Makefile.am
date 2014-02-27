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

noinst_LTLIBRARIES = \
	libtvfunc.la \
	libtvfunc_d.la \
	libtvfunc_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	w_table.h \
	TvFunc.cc \
	TvFuncM.cc


############################################################
## libtvfunc.la 用の変数の指定
############################################################

libtvfunc_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libtvfunc_la_SOURCES = $(COMMON_SOURCES)

libtvfunc_la_LIBADD =

libtvfunc_la_LDFLAGS =


############################################################
## libtvfunc_d.la 用の変数の指定
############################################################

libtvfunc_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libtvfunc_d_la_SOURCES = $(COMMON_SOURCES)

libtvfunc_d_la_LIBADD =

libtvfunc_d_la_LDFLAGS =


############################################################
## libtvfunc_p.la 用の変数の指定
############################################################

libtvfunc_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libtvfunc_p_la_SOURCES = $(COMMON_SOURCES)

libtvfunc_p_la_LIBADD =

libtvfunc_p_la_LDFLAGS =
