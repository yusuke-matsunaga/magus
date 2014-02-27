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
	libmisc.la \
	libmisc_d.la \
	libmisc_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	@POPT_INCLUDES@


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	Binder.cc \
	DlList.cc \
	PoptMainApp.cc \
	RandGen.cc \
	RefCount.cc \
	Scanner.cc \
	StopWatch.cc

COMMON_LDFLAGS = \
	@POPT_LIBS@


############################################################
## libmisc.la 用の変数の定義
############################################################

libmisc_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libmisc_la_SOURCES = $(COMMON_SOURCES)

libmisc_la_LIBADD =

libmisc_la_LDFLAGS = $(COMMON_LDFLAGS)


############################################################
## libmisc_d.la 用の変数の定義
############################################################

libmisc_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libmisc_d_la_SOURCES = $(COMMON_SOURCES)

libmisc_d_la_LIBADD =

libmisc_d_la_LDFLAGS = $(COMMON_LDFLAGS)


############################################################
## libmisc_p.la 用の変数の定義
############################################################

libmisc_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libmisc_p_la_SOURCES = $(COMMON_SOURCES)

libmisc_p_la_LIBADD =

libmisc_p_la_LDFLAGS = $(COMMON_LDFLAGS)
