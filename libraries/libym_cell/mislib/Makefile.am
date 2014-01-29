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

noinst_LTLIBRARIES = libym_cell_mislib.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

AM_YFLAGS = -d -v


############################################################
## libym_cell_mislib.la 用の変数の指定
############################################################

libym_cell_mislib_la_SOURCES = \
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

libym_cell_mislib_la_LIBADD =

libym_cell_mislib_la_LDFLAGS =
