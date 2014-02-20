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

noinst_LTLIBRARIES = libym_sat_dimacs.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## libym_sat_dimacs.la 用の変数の指定
############################################################

libym_sat_dimacs_la_SOURCES = \
	DimacsParser.cc \
	DimacsParserImpl.h \
	DimacsScanner.h \
	DimacsScanner.cc \
	DimacsVerifier.cc \
	SatDimacsHandler.cc

libym_sat_dimacs_la_LIBADD =

libym_sat_dimacs_la_LDFLAGS =
