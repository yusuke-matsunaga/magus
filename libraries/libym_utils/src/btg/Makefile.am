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
	libbtg.la \
	libbtg_d.la \
	libbtg_p.la


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
	BtgEdge.h \
	BtgHeapTree.h \
	BtgHeapTree.cc \
	BtgMatch.cc \
	BtgNode.h


############################################################
## libbtg.la 用の変数の定義
############################################################

libbtg_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libbtg_la_SOURCES = $(COMMON_SOURCES)

libbtg_la_LIBADD = \
	$(AM_LIBADD)

libbtg_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libbtg_d.la 用の変数の定義
############################################################

libbtg_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libbtg_d_la_SOURCES = $(COMMON_SOURCES)

libbtg_d_la_LIBADD = \
	$(AM_LIBADD)

libbtg_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libbtg_p.la 用の変数の定義
############################################################

libbtg_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libbtg_p_la_SOURCES = $(COMMON_SOURCES)

libbtg_p_la_LIBADD = \
	$(AM_LIBADD)

libbtg_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
