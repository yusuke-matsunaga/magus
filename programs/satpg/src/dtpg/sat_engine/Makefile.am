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
	libatpg_sat_engine.la \
	libatpg_sat_engine_d.la \
	libatpg_sat_engine_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	satpg_engine_nsdef.h \
	InputLiteral.h \
	InputLiteral.cc \
	SatEngineImpl.h \
	SatEngineImpl.cc


############################################################
## libatpg_sat_engine.la 用の変数の定義
############################################################

libatpg_sat_engine_la_CXXFLAGS = -O2

libatpg_sat_engine_la_SOURCES = $(COMMON_SOURCES)

libatpg_sat_engine_la_LIBADD =

libatpg_sat_engine_la_LDFLAGS =


############################################################
## libatpg_sat_engine.la 用の変数の定義
############################################################

libatpg_sat_engine_d_la_CXXFLAGS = -O2 -g

libatpg_sat_engine_d_la_SOURCES = $(COMMON_SOURCES)

libatpg_sat_engine_d_la_LIBADD =

libatpg_sat_engine_d_la_LDFLAGS =


############################################################
## libatpg_sat_engine_p.la 用の変数の定義
############################################################

libatpg_sat_engine_p_la_CXXFLAGS = -O2 -pg

libatpg_sat_engine_p_la_SOURCES = $(COMMON_SOURCES)

libatpg_sat_engine_p_la_LIBADD =

libatpg_sat_engine_p_la_LDFLAGS =
