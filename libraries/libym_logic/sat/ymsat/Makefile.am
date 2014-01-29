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

noinst_LTLIBRARIES = libym_sat_ymsat.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_sat_ymsat.la 用の変数の指定
############################################################

libym_sat_ymsat_la_SOURCES = \
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

libym_sat_ymsat_la_LIBADD =

libym_sat_ymsat_la_LDFLAGS =
