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
	libym_sat_dimacs.la \
	libym_sat_dimacs_d.la \
	libym_sat_dimacs_p.la


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
	DimacsParser.cc \
	DimacsParserImpl.h \
	DimacsScanner.h \
	DimacsScanner.cc \
	DimacsVerifier.cc \
	SatDimacsHandler.cc


############################################################
## libym_sat_dimacs.la 用の変数の指定
############################################################

libym_sat_dimacs_la_CXXFLAGS = -O2

libym_sat_dimacs_la_SOURCES = $(COMMON_SOURCES)

libym_sat_dimacs_la_LIBADD =

libym_sat_dimacs_la_LDFLAGS =


############################################################
## libym_sat_dimacs_d.la 用の変数の指定
############################################################

libym_sat_dimacs_d_la_CXXFLAGS = -O2 -g

libym_sat_dimacs_d_la_SOURCES = $(COMMON_SOURCES)

libym_sat_dimacs_d_la_LIBADD =

libym_sat_dimacs_d_la_LDFLAGS =


############################################################
## libym_sat_dimacs_p.la 用の変数の指定
############################################################

libym_sat_dimacs_p_la_CXXFLAGS = -O2 -pg

libym_sat_dimacs_p_la_SOURCES = $(COMMON_SOURCES)

libym_sat_dimacs_p_la_LIBADD =

libym_sat_dimacs_p_la_LDFLAGS =
