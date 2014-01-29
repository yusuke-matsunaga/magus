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

noinst_LTLIBRARIES = libym_cmn.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_cmn.la 用の変数の定義
############################################################

libym_cmn_la_SOURCES = \
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

libym_cmn_la_LIBADD =

libym_cmn_la_LDFLAGS =
