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
	libcmn.la \
	libcmn_d.la \
	libcmn_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/verilog \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	CmnDffCell.cc \
	CmnDumper.cc \
	CmnLatchCell.cc \
	CmnMgr.cc \
	CmnMgrImpl.h \
	CmnMgrImpl.cc \
	CmnNode.cc \
	CmnNodeDffIn.h \
	CmnNodeDffOut.h \
	CmnNodeInput.h \
	CmnNodeLatchIn.h \
	CmnNodeLatchOut.h \
	CmnNodeLogic.h \
	CmnNodeOutput.h \
	CmnNodePI.h \
	CmnNodePO.h \
	CmnPort.cc \
	CmnBlifWriter.cc \
	CmnVerilogWriter.cc \
	BlifWriterImpl.h \
	BlifWriterImpl.cc \
	VerilogWriterImpl.h \
	VerilogWriterImpl.cc


############################################################
## libcmn.la 用の変数の定義
############################################################

libcmn_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libcmn_la_SOURCES = $(COMMON_SOURCES)

libcmn_la_LIBADD =

libcmn_la_LDFLAGS =


############################################################
## libcmn_d.la 用の変数の定義
############################################################

libcmn_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libcmn_d_la_SOURCES = $(COMMON_SOURCES)

libcmn_d_la_LIBADD =

libcmn_d_la_LDFLAGS =


############################################################
## libcmn_p.la 用の変数の定義
############################################################

libcmn_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libcmn_p_la_SOURCES = $(COMMON_SOURCES)

libcmn_p_la_LIBADD =

libcmn_p_la_LDFLAGS =
