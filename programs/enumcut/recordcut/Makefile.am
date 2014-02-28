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
	recordcut


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
	Lut443Match.h \
	Lut443Match.cc

recordcut_LDADD = \
	$(top_builddir)/libgbm/libgbm.la \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_popt/src/libym_popt.la
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la \
	$(AM_LDADD)

recordcut_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Lut443MatchTest 用の変数の定義
############################################################

Lut443MatchTest_SOURCES = \
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
