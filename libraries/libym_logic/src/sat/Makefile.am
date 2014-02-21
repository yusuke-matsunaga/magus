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
	libym_logic_sat.la \
	libym_logic_sat_d.la \
	libym_logic_sat_p.la


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
## libym_logic_sat.la 用の変数の指定
############################################################

libym_logic_sat_la_CXXFLAGS = -O2

libym_logic_sat_la_SOURCES = $(COMMON_SOURCES)

libym_logic_sat_la_LIBADD = \
	ymsat/libym_sat_ymsat.la \
	MiniSat/libym_minisat.la \
	dimacs/libym_sat_dimacs.la

libym_logic_sat_la_LDFLAGS =


############################################################
## libym_logic_sat_d.la 用の変数の指定
############################################################

libym_logic_sat_d_la_CXXFLAGS = -O2 -g

libym_logic_sat_d_la_SOURCES = $(COMMON_SOURCES)

libym_logic_sat_d_la_LIBADD = \
	ymsat/libym_sat_ymsat_d.la \
	MiniSat/libym_minisat_d.la \
	dimacs/libym_sat_dimacs_d.la

libym_logic_sat_d_la_LDFLAGS =


############################################################
## libym_logic_sat_p.la 用の変数の指定
############################################################

libym_logic_sat_p_la_CXXFLAGS = -O2 -pg

libym_logic_sat_p_la_SOURCES = $(COMMON_SOURCES)

libym_logic_sat_p_la_LIBADD = \
	ymsat/libym_sat_ymsat_p.la \
	MiniSat/libym_minisat_p.la \
	dimacs/libym_sat_dimacs_p.la

libym_logic_sat_p_la_LDFLAGS =
