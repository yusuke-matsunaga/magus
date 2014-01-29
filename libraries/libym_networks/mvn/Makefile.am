## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	verilog \
	.


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

noinst_LTLIBRARIES = libym_mvn.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_mvn.la 用の変数の定義
############################################################

libym_mvn_la_SOURCES = \
	MvnCaseEq.h \
	MvnCaseEq.cc \
	MvnCellNode.h \
	MvnCellNode.cc \
	MvnConst.h \
	MvnConst.cc \
	MvnConstBitSelect.h \
	MvnConstBitSelect.cc \
	MvnConstPartSelect.h \
	MvnConstPartSelect.cc \
	MvnDff.h \
	MvnDff.cc \
	MvnDumper.cc \
	MvnMgr.cc \
	MvnModule.cc \
	MvnNode.cc \
	MvnNodeBase.h \
	MvnNodeBase.cc \
	MvnPort.cc \
	MvnVerilogWriter.cc \
	VerilogWriterImpl.h \
	VerilogWriterImpl.cc

libym_mvn_la_LIBADD = \
	verilog/libym_mvn_verilog.la

libym_mvn_la_LDFLAGS =
