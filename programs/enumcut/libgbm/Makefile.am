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
	libgbm.la \
	libgbm_d.la \
	libgbm_p.la


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
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	@GMP_INCLUDES@


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libgbm.la 用の変数の定義
############################################################

libgbm_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libgbm_la_SOURCES = \
	$(COMMON_SOURCES)

libgbm_la_LIBADD =

libgbm_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libgbm_d.la 用の変数の定義
############################################################

libgbm_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libgbm_d_la_SOURCES = \
	$(COMMON_SOURCES)

libgbm_d_la_LIBADD =

libgbm_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libgbm_p.la 用の変数の定義
############################################################

libgbm_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libgbm_p_la_SOURCES = \
	$(COMMON_SOURCES)

libgbm_p_la_LIBADD =

libgbm_p_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## GbmSolverTest 用の変数の定義
############################################################

GbmSolverTest_SOURCES = \
	GbmSolverTest.cc

GbmSolverTest_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

GbmSolverTest_LDFLAGS =


############################################################
## lut4match_test 用の変数の定義
############################################################

lut4match_test_SOURCES = \
	lut4match_test.cc

lut4match_test_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

lut4match_test_LDFLAGS =
