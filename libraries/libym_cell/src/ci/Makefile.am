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
	libym_cell_ci.la \
	libym_cell_ci_d.la \
	libym_cell_ci_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CiBundle.h \
	CiBundle.cc \
	CiBus.h \
	CiBus.cc \
	CiBusType.h \
	CiBusType.cc \
	CiClass.h \
	CiClass.cc \
	CiCell.h \
	CiCell.cc \
	CiCellHash.h \
	CiCellHash.cc \
	CiLogicCell.h \
	CiLogicCell.cc \
	CiFFCell.h \
	CiFFCell.cc \
	CiFsmCell.h \
	CiFsmCell.cc \
	CiLatchCell.h \
	CiLatchCell.cc \
	CiGroup.h \
	CiGroup.cc \
	CiLibrary.h \
	CiLibrary.cc \
	CiLut.h \
	CiLut.cc \
	CiLutHash.h \
	CiLutHash.cc \
	CiLutTemplate.h \
	CiLutTemplate.cc \
	CiPatGraph.h \
	CiPatGraph.cc \
	CiPatMgr.h \
	CiPatMgr.cc \
	CiPin.h \
	CiPin.cc \
	CiPinHash.h \
	CiPinHash.cc \
	CiTiming.h \
	CiTiming.cc


############################################################
## libym_cell_ci.la 用の変数の指定
############################################################

libym_cell_ci_la_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

libym_cell_ci_la_SOURCES = \
	$(COMMON_SOURCES)

libym_cell_ci_la_LIBADD =

libym_cell_ci_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libym_cell_ci_d.la 用の変数の指定
############################################################

libym_cell_ci_d_la_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

libym_cell_ci_d_la_SOURCES = \
	$(COMMON_SOURCES)

libym_cell_ci_d_la_LIBADD =

libym_cell_ci_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libym_cell_ci_p.la 用の変数の指定
############################################################

libym_cell_ci_p_la_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

libym_cell_ci_p_la_SOURCES = \
	$(COMMON_SOURCES)

libym_cell_ci_p_la_LIBADD =

libym_cell_ci_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
