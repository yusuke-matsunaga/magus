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
	libminisat2.la \
	libminisat2_d.la \
	libminisat2_p.la


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
	SatSolverMiniSat2.h \
	SatSolverMiniSat2.cc \
	Alg.h \
	Alloc.h \
	Heap.h \
	IntTypes.h \
	Map.h \
	Options.h \
	Options.cc \
	ParseUtils.h \
	Queue.h \
	Solver.cc \
	Solver.h \
	SolverTypes.h \
	Sort.h \
	System.cc \
	System.h \
	Vec.h \
	XAlloc.h


############################################################
## libminisat2.la 用の変数の指定
############################################################

libminisat2_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libminisat2_la_SOURCES = \
	$(COMMON_SOURCES)

libminisat2_la_LIBADD =

libminisat2_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libminisat2_d.la 用の変数の指定
############################################################

libminisat2_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libminisat2_d_la_SOURCES = \
	$(COMMON_SOURCES)

libminisat2_d_la_LIBADD =

libminisat2_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libminisat2_p.la 用の変数の指定
############################################################

libminisat2_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libminisat2_p_la_SOURCES = \
	$(COMMON_SOURCES)

libminisat2_p_la_LIBADD =

libminisat2_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
