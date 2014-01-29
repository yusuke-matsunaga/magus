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
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = \
	libgbm.la

noinst_PROGRAMS = \
	rec \
	recordcut \
	GbmSolverTest \
	lut4match_test \
	Lut443MatchTest


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include


############################################################
## libgbm.la 用の変数の定義
############################################################

libgbm_la_SOURCES = \
	GbmEngine.h \
	GbmEngine.cc \
	GbmEngineBinary.h \
	GbmEngineBinary.cc \
	GbmEngineOneHot.h \
	GbmEngineOneHot.cc \
	GbmEngineEnum.h \
	GbmEngineEnum.cc \
	GbmSolver.h \
	GbmSolver.cc \
	GbmNaiveBinary.h \
	GbmNaiveBinary.cc \
	GbmNaiveEnum.h \
	GbmNaiveEnum.cc \
	GbmNaiveOneHot.h \
	GbmNaiveOneHot.cc \
	GbmIncrEnum.h \
	GbmIncrEnum.cc \
	GbmIncrBinary.h \
	GbmIncrBinary.cc \
	GbmIncrOneHot.h \
	GbmIncrOneHot.cc \
	GbmCegarBinary.h \
	GbmCegarBinary.cc \
	GbmCegarOneHot.h \
	GbmCegarOneHot.cc \
	GbmCegarEnum.h \
	GbmCegarEnum.cc \
	RcfNetwork.h \
	RcfNetwork.cc \
	RcfNode.h \
	RcfNodeImpl.h \
	RcfNodeImpl.cc \
	RcfNodeHandle.h \
	Lut443Match.h \
	Lut443Match.cc

libgbm_la_LIBADD =

libgbm_la_LDFLAGS =


############################################################
## rec 用の変数の定義
############################################################

rec_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES)

rec_SOURCES = \
	FuncHash.h \
	FuncHash.cc \
	FuncMgr.h \
	FuncMgr.cc \
	FuncRec.h \
	FuncRec.cc \
	RecCmd.h \
	RecCmd.cc \
	tclAppInit.cc

rec_LDADD = \
	libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

rec_LDFLAGS =


############################################################
## recordcut 用の変数の定義
############################################################

recordcut_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	@POPT_INCLUDES@

recordcut_SOURCES = \
	recordcut.cc \
	Cut.h \
	CutHash.h \
	CutHash.cc \
	CutMgr.h \
	CutMgr.cc \
	FuncMgr.h \
	FuncMgr.cc \
	FuncRec.h \
	FuncRec.cc

recordcut_LDADD = \
	libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

recordcut_LDFLAGS = \
	@POPT_LIBS@


############################################################
## GbmSolverTest 用の変数の定義
############################################################

GbmSolverTest_SOURCES = \
	GbmSolverTest.cc

GbmSolverTest_LDADD = \
	libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

GbmSolverTest_LDFLAGS =


############################################################
## Lut443MatchTest 用の変数の定義
############################################################

Lut443MatchTest_SOURCES = \
	Lut443MatchTest.cc

Lut443MatchTest_LDADD = \
	libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

Lut443MatchTest_LDFLAGS =


############################################################
## lut4match_test 用の変数の定義
############################################################

lut4match_test_SOURCES = \
	lut4match_test.cc

lut4match_test_LDADD = \
	libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

lut4match_test_LDFLAGS =
