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
	libymsat.la \
	libymsat_p.la \
	libymsat_d.la


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
## libymsat.la 用の変数の指定
############################################################

libymsat_la_CXXFLAGS = -O2

libymsat_la_SOURCES = $(COMMON_SOURCES)

libymsat_la_LIBADD =

libymsat_la_LDFLAGS =


############################################################
## libymsat_d.la 用の変数の指定
############################################################

libymsat_d_la_CXXFLAGS = -O2 -g

libymsat_d_la_SOURCES = $(COMMON_SOURCES)

libymsat_d_la_LIBADD =

libymsat_d_la_LDFLAGS =


############################################################
## libymsat_p.la 用の変数の指定
############################################################

libymsat_p_la_CXXFLAGS = -O2 -pg

libymsat_p_la_SOURCES = $(COMMON_SOURCES)

libymsat_p_la_LIBADD =

libymsat_p_la_LDFLAGS =
