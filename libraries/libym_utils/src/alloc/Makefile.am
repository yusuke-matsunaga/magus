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
	liballoc.la \
	liballoc_d.la \
	liballoc_p.la


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
	Alloc.cc \
	FragAlloc.cc \
	SimpleAlloc.cc \
	UnitAlloc.cc


############################################################
## liballoc.la 用の変数の定義
############################################################

liballoc_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

liballoc_la_SOURCES = $(COMMON_SOURCES)

liballoc_la_LIBADD =

liballoc_la_LDFLAGS =


############################################################
## liballoc_d.la 用の変数の定義
############################################################

liballoc_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

liballoc_d_la_SOURCES = $(COMMON_SOURCES)

liballoc_d_la_LIBADD = $(YM_CXXDBGFLAGS)

liballoc_d_la_LDFLAGS =


############################################################
## liballoc_p.la 用の変数の定義
############################################################

liballoc_p_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

liballoc_p_la_SOURCES = $(COMMON_SOURCES)

liballoc_p_la_LIBADD =

liballoc_p_la_LDFLAGS =
