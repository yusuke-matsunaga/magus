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
	libgen.la \
	libgen_d.la \
	libgen_p.la


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
	CombiGen.cc \
	GenBase.cc \
	MultiCombiGen.cc \
	MultiGenBase.cc \
	MultiSetCombiGen.cc \
	MultiSetGenBase.cc \
	MultiSetPermGen.cc \
	MultiPermGen.cc \
	PermGen.cc


############################################################
## libgen.la 用の変数の定義
############################################################

libgen_la_CXXFLAGS = -O2

libgen_la_SOURCES = $(COMMON_SOURCES)

libgen_la_LIBADD =

libgen_la_LDFLAGS =


############################################################
## libgen_d.la 用の変数の定義
############################################################

libgen_d_la_CXXFLAGS = -O2 -g

libgen_d_la_SOURCES = $(COMMON_SOURCES)

libgen_d_la_LIBADD =

libgen_d_la_LDFLAGS =


############################################################
## libgen_p.la 用の変数の定義
############################################################

libgen_p_la_CXXFLAGS = -O2 -pg

libgen_p_la_SOURCES = $(COMMON_SOURCES)

libgen_p_la_LIBADD =

libgen_p_la_LDFLAGS =
