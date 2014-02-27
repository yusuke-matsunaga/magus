## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	ymsat \
	MiniSat \
	dimacs \
	.


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
	libsat.la \
	libsat_d.la \
	libsat_p.la


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
	SatSolver.cc \
	SatMsgHandlerImpl1.cc


############################################################
## libsat.la 用の変数の指定
############################################################

libsat_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libsat_la_SOURCES = $(COMMON_SOURCES)

libsat_la_LIBADD = \
	ymsat/libymsat.la \
	MiniSat/libminisat.la \
	dimacs/libdimacs.la

libsat_la_LDFLAGS =


############################################################
## libsat_d.la 用の変数の指定
############################################################

libsat_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libsat_d_la_SOURCES = $(COMMON_SOURCES)

libsat_d_la_LIBADD = \
	ymsat/libymsat_d.la \
	MiniSat/libminisat_d.la \
	dimacs/libdimacs_d.la

libsat_d_la_LDFLAGS =


############################################################
## libsat_p.la 用の変数の指定
############################################################

libsat_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libsat_p_la_SOURCES = $(COMMON_SOURCES)

libsat_p_la_LIBADD = \
	ymsat/libymsat_p.la \
	MiniSat/libminisat_p.la \
	dimacs/libdimacs_p.la

libsat_p_la_LDFLAGS =
