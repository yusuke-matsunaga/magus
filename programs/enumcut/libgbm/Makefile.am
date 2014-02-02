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


############################################################
## build ターゲットの指定
############################################################

noinst_PROGRAMS = \
	GbmSolverTest \
	lut4match_test


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	@GMP_INCLUDES@


############################################################
## libgbm.la 用の変数の定義
############################################################

libgbm_la_SOURCES = \
	GbmSatEngine.h \
	GbmSatEngine.cc \
	GbmSatEngineWithEncoder.h \
	GbmSatEngineBinary.h \
	GbmSatEngineBinary.cc \
	GbmSatEngineOneHot.h \
	GbmSatEngineOneHot.cc \
	GbmSatEngineEnum.h \
	GbmSatEngineEnum.cc \
	GbmBddEngineOneHot.h \
	GbmBddEngineOneHot.cc \
	GbmBddEngineBinary.h \
	GbmBddEngineBinary.cc \
	GbmBddEngineEnum.h \
	GbmBddEngineEnum.cc \
	GbmSolver.cc \
	GbmSatNaiveBinary.h \
	GbmSatNaiveBinary.cc \
	GbmSatNaiveEnum.h \
	GbmSatNaiveEnum.cc \
	GbmSatNaiveOneHot.h \
	GbmSatNaiveOneHot.cc \
	GbmSatCegarBinary.h \
	GbmSatCegarBinary.cc \
	GbmSatCegarOneHot.h \
	GbmSatCegarOneHot.cc \
	GbmSatCegarEnum.h \
	GbmSatCegarEnum.cc \
	GbmBddCegarOneHot.h \
	GbmBddCegarOneHot.cc \
	GbmBddCegarBinary.h \
	GbmBddCegarBinary.cc \
	GbmBddCegarEnum.h \
	GbmBddCegarEnum.cc \
	RcfNetwork.cc \
	RcfNode.h \
	RcfNodeImpl.h \
	RcfNodeImpl.cc

libgbm_la_LIBADD =

libgbm_la_LDFLAGS =


############################################################
## GbmSolverTest 用の変数の定義
############################################################

GbmSolverTest_SOURCES = \
	GbmSolverTest.cc

GbmSolverTest_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

GbmSolverTest_LDFLAGS =


############################################################
## lut4match_test 用の変数の定義
############################################################

lut4match_test_SOURCES = \
	lut4match_test.cc

lut4match_test_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

lut4match_test_LDFLAGS =
