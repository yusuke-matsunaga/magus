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

noinst_LTLIBRARIES = libatpg_sat_engine.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## libatpg_sat_engine.la 用の変数の定義
############################################################

libatpg_sat_engine_la_SOURCES = \
	satpg_engine_nsdef.h \
	InputLiteral.h \
	InputLiteral.cc \
	SatEngineImpl.h \
	SatEngineImpl.cc

libatpg_sat_engine_la_LIBADD =

libatpg_sat_engine_la_LDFLAGS =
