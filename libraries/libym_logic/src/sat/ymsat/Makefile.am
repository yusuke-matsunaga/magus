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
	libym_sat_ymsat.la \
	libym_sat_ymsat_p.la \
	libym_sat_ymsat_d.la


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
	AssignList.h \
	AssignList.cc \
	SaBase.h \
	SaBase.cc \
	SaFactory.cc \
	SaUIP1.h \
	SaUIP1.cc \
	SaSimple.h \
	SaSimple.cc \
	SatAnalyzer.h \
	SatClause.h \
	SatClause.cc \
	YmSat.h \
	YmSat.cc \
	YmSatR.h \
	YmSatR.cc


############################################################
## libym_sat_ymsat.la 用の変数の指定
############################################################

libym_sat_ymsat_la_CXXFLAGS = -O2

libym_sat_ymsat_la_SOURCES = $(COMMON_SOURCES)

libym_sat_ymsat_la_LIBADD =

libym_sat_ymsat_la_LDFLAGS =


############################################################
## libym_sat_ymsat_d.la 用の変数の指定
############################################################

libym_sat_ymsat_d_la_CXXFLAGS = -O2 -g

libym_sat_ymsat_d_la_SOURCES = $(COMMON_SOURCES)

libym_sat_ymsat_d_la_LIBADD =

libym_sat_ymsat_d_la_LDFLAGS =


############################################################
## libym_sat_ymsat_p.la 用の変数の指定
############################################################

libym_sat_ymsat_p_la_CXXFLAGS = -O2 -pg

libym_sat_ymsat_p_la_SOURCES = $(COMMON_SOURCES)

libym_sat_ymsat_p_la_LIBADD =

libym_sat_ymsat_p_la_LDFLAGS =
