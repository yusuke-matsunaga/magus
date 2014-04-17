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

noinst_PROGRAMS = \
	rec \
	Lut443MatchTest \
	recordcut \
	recordcut_d


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_tclpp/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	@GMP_INCLUDES@


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
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/src/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

rec_LDFLAGS =


############################################################
## recordcut 用の変数の定義
############################################################

recordcut_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_popt/include

recordcut_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

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
	FuncRec.cc \
	LnMatch.h \
	LnMatch.cc \
	Lut443Match.h \
	Lut443Match.cc \
	Lut444Match.h \
	Lut444Match.cc

recordcut_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

recordcut_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## recordcut_d 用の変数の定義
############################################################

recordcut_d_CPPFLAGS = \
	$(AM_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_popt/include

recordcut_d_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

recordcut_d_SOURCES = \
	$(recordcut_SOURCES)

recordcut_d_LDADD = \
	$(top_builddir)/libgbm/libgbm_d.la \
	$(top_builddir)/libec/libec_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt_d.la
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_d.la

recordcut_d_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Lut443MatchTest 用の変数の定義
############################################################

Lut443MatchTest_SOURCES = \
	LnMatch.h \
	LnMatch.cc \
	Lut443Match.h \
	Lut443Match.cc \
	Lut443MatchTest.cc

Lut443MatchTest_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la

Lut443MatchTest_LDFLAGS =
