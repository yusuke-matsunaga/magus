## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	mislib_grammer.cc \
	mislib_grammer.hh \
	y.output \
	mislib_grammer.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = \
	libym_cell_mislib.la \
	libym_cell_mislib_d.la \
	libym_cell_mislib_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include

AM_YFLAGS = -d -v


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CellMislibReader.cc \
	mislib_nsdef.h \
	mislib_grammer.yy \
	MislibScanner.h \
	MislibScanner.cc \
	MislibMgr.h \
	MislibMgr.cc \
	MislibMgrImpl.h \
	MislibMgrImpl.cc \
	MislibParser.h \
	MislibParser.cc \
	MislibParserImpl.h \
	MislibParserImpl.cc \
	MislibNode.h \
	MislibNodeImpl.h \
	MislibNodeImpl.cc \
	MislibNum.h \
	MislibNum.cc \
	MislibStr.h \
	MislibStr.cc \
	MislibSymbol.h \
	MislibSymbol.cc \
	MislibConst.h \
	MislibConst.cc \
	MislibList.h \
	MislibList.cc \
	MislibNot.h \
	MislibNot.cc \
	MislibBop.h \
	MislibBop.cc \
	MislibGate.h \
	MislibGate.cc \
	MislibPin.h \
	MislibPin.cc


############################################################
## libym_cell_mislib.la 用の変数の指定
############################################################

libym_cell_mislib_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_cell_mislib_la_SOURCES = $(COMMON_SOURCES)

libym_cell_mislib_la_LIBADD =

libym_cell_mislib_la_LDFLAGS =


############################################################
## libym_cell_mislib_d.la 用の変数の指定
############################################################

libym_cell_mislib_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_cell_mislib_d_la_SOURCES = $(COMMON_SOURCES)

libym_cell_mislib_d_la_LIBADD =

libym_cell_mislib_d_la_LDFLAGS =


############################################################
## libym_cell_mislib_p.la 用の変数の指定
############################################################

libym_cell_mislib_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_cell_mislib_p_la_SOURCES = $(COMMON_SOURCES)

libym_cell_mislib_p_la_LIBADD =

libym_cell_mislib_p_la_LDFLAGS =
