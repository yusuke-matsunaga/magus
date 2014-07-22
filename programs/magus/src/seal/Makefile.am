## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	. \
	tests


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

noinst_LTLIBRARIES = libseal.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(GMP_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_tclpp/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## libseal.la 用の変数の定義
############################################################

libseal_la_SOURCES = \
	BddFsm.h \
	BddFsm.cc \
	IdxMapper.h \
	fsm_analysis.cc \
	LogicSim.h \
	LogicSim.cc \
	fsm_analysis2.cc \
	Matrix.h \
	Matrix.cc \
	SMatrix.h \
	SMatrix.cc \
	Dfs.h \
	Dfs.cc \
	gauss_elimination.cc \
	lu_decomp.cc \
	MCAnalysis.cc

libseal_la_LIBADD =

libseal_la_LDFLAGS = \
	$(GMP_LIBS)
