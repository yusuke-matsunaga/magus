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
	libmfset.la \
	libmfset_d.la \
	libmfset_p.la


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
	MFSet.cc


############################################################
## libmfset.la 用の変数の定義
############################################################

libmfset_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libmfset_la_SOURCES = $(COMMON_SOURCES)

libmfset_la_LIBADD = \
	$(AM_LIBADD)

libmfset_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libmfset_d.la 用の変数の定義
############################################################

libmfset_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libmfset_d_la_SOURCES = $(COMMON_SOURCES)

libmfset_d_la_LIBADD = \
	$(AM_LIBADD)

libmfset_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libmfset_p.la 用の変数の定義
############################################################

libmfset_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libmfset_p_la_SOURCES = $(COMMON_SOURCES)

libmfset_p_la_LIBADD = \
	$(AM_LIBADD)

libmfset_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
