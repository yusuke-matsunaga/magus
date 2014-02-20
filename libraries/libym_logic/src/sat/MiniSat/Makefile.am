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

noinst_LTLIBRARIES = libym_minisat.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## libym_minisat.la 用の変数の指定
############################################################

libym_minisat_la_SOURCES = \
	SatSolverMiniSat.h \
	SatSolverMiniSat.cc \
	Global.h \
	Heap.h \
	Solver.C \
	Solver.h \
	SolverTypes.h \
	Sort.h \
	VarOrder.h

libym_minisat_la_LIBADD =

libym_minisat_la_LDFLAGS =
