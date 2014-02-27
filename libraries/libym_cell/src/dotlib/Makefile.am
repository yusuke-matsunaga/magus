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
	libym_cell_dotlib.la \
	libym_cell_dotlib_d.la \
	libym_cell_dotlib_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CellDotlibReader.cc \
	dotlib_nsdef.h \
	dotlib_int.h \
	DotlibParser.h \
	DotlibParser.cc \
	DotlibParserImpl.h \
	DotlibParserImpl.cc \
	DotlibScanner.h \
	DotlibScanner.cc \
	DotlibHandler.h \
	DotlibHandler.cc \
	SimpleHandler.h \
	SimpleHandler.cc \
	ExprHandler.h \
	ExprHandler.cc \
	FuncHandler.h \
	FuncHandler.cc \
	FhScanner.h \
	FhScanner.cc \
	ComplexHandler.h \
	ComplexHandler.cc \
	DefineHandler.h \
	DefineHandler.cc \
	GroupHandler.h \
	GroupHandler.cc \
	HandlerFactory.h \
	HandlerFactory.cc \
	DotlibMgr.h \
	DotlibMgr.cc \
	DotlibMgrImpl.h \
	DotlibMgrImpl.cc \
	DotlibNode.h \
	DotlibAttr.h \
	DotlibAttr.cc \
	DotlibNodeImpl.h \
	DotlibNodeImpl.cc \
	DotlibNode.cc \
	DotlibAttrMap.h \
	DotlibAttrMap.cc \
	DotlibLibrary.h \
	DotlibLibrary.cc \
	DotlibCell.h \
	DotlibCell.cc \
	DotlibFL.h \
	DotlibFL.cc \
	DotlibFF.h \
	DotlibFF.cc \
	DotlibLatch.h \
	DotlibLatch.cc \
	DotlibPin.h \
	DotlibPin.cc \
	DotlibTiming.h \
	DotlibTiming.cc \
	DotlibTemplate.h \
	DotlibTemplate.cc \
	DotlibLut.h \
	DotlibLut.cc


############################################################
## libym_cell_dotlib.la 用の変数の指定
############################################################

libym_cell_dotlib_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_cell_dotlib_la_SOURCES = $(COMMON_SOURCES)

libym_cell_dotlib_la_LDFLAGS =

libym_cell_dotlib_la_LIBADD =


############################################################
## libym_cell_dotlib_d.la 用の変数の指定
############################################################

libym_cell_dotlib_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_cell_dotlib_d_la_SOURCES = $(COMMON_SOURCES)

libym_cell_dotlib_d_la_LDFLAGS =

libym_cell_dotlib_d_la_LIBADD =


############################################################
## libym_cell_dotlib_p.la 用の変数の指定
############################################################

libym_cell_dotlib_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_cell_dotlib_p_la_SOURCES = $(COMMON_SOURCES)

libym_cell_dotlib_p_la_LDFLAGS =

libym_cell_dotlib_p_la_LIBADD =
