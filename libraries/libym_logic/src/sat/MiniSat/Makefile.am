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
	libminisat.la \
	libminisat_d.la \
	libminisat_p.la


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
	SatSolverMiniSat.h \
	SatSolverMiniSat.cc \
	Global.h \
	Heap.h \
	Solver.C \
	Solver.h \
	SolverTypes.h \
	Sort.h \
	VarOrder.h


############################################################
## libminisat.la 用の変数の指定
############################################################

libminisat_la_CXXFLAGS = -O2

libminisat_la_SOURCES = $(COMMON_SOURCES)

libminisat_la_LIBADD =

libminisat_la_LDFLAGS =


############################################################
## libminisat_d.la 用の変数の指定
############################################################

libminisat_d_la_CXXFLAGS = -O2 -g

libminisat_d_la_SOURCES = $(COMMON_SOURCES)

libminisat_d_la_LIBADD =

libminisat_d_la_LDFLAGS =


############################################################
## libminisat_p.la 用の変数の指定
############################################################

libminisat_p_la_CXXFLAGS = -O2 -pg

libminisat_p_la_SOURCES = $(COMMON_SOURCES)

libminisat_p_la_LIBADD =

libminisat_p_la_LDFLAGS =
