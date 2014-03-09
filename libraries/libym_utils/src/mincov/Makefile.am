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
	libmincov.la \
	libmincov_d.la \
	libmincov_p.la


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
	MaxClique.cc \
	MaxClique.h \
	MincovCost.cc \
	MincovMatrix.cc \
	MincovSolver.cc


############################################################
## libmincov.la 用の変数の定義
############################################################

libmincov_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libmincov_la_SOURCES = \
	$(COMMON_SOURCES)

libmincov_la_LIBADD =

libmincov_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libmincov_d.la 用の変数の定義
############################################################

libmincov_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libmincov_d_la_SOURCES = \
	$(COMMON_SOURCES)

libmincov_d_la_LIBADD =

libmincov_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libmincov_p.la 用の変数の定義
############################################################

libmincov_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libmincov_p_la_SOURCES = \
	$(COMMON_SOURCES)

libmincov_p_la_LIBADD =

libmincov_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
