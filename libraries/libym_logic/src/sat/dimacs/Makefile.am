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
	libdimacs.la \
	libdimacs_d.la \
	libdimacs_p.la


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
## libdimacs.la 用の変数の指定
############################################################

libdimacs_la_CXXFLAGS = -O2

libdimacs_la_SOURCES = $(COMMON_SOURCES)

libdimacs_la_LIBADD =

libdimacs_la_LDFLAGS =


############################################################
## libdimacs_d.la 用の変数の指定
############################################################

libdimacs_d_la_CXXFLAGS = -O2 -g

libdimacs_d_la_SOURCES = $(COMMON_SOURCES)

libdimacs_d_la_LIBADD =

libdimacs_d_la_LDFLAGS =


############################################################
## libdimacs_p.la 用の変数の指定
############################################################

libdimacs_p_la_CXXFLAGS = -O2 -pg

libdimacs_p_la_SOURCES = $(COMMON_SOURCES)

libdimacs_p_la_LIBADD =

libdimacs_p_la_LDFLAGS =
