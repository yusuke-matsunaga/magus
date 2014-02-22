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

noinst_LTLIBRARIES = \
	libmvn.la \
	libmvn_d.la \
	libmvn_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_srcdir)/../libym_cell/include \
	-I$(top_srcdir)/../libym_verilog/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## libmvn.la 用の変数の定義
############################################################

COMMON_SOURCES = \
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


############################################################
## libmvn.la 用の変数の定義
############################################################

libmvn_la_CXXFLAGS = -O2

libmvn_la_SOURCES = $(COMMON_SOURCES)

libmvn_la_LIBADD = \
	verilog/libmvn_verilog.la

libmvn_la_LDFLAGS =


############################################################
## libmvn_d.la 用の変数の定義
############################################################

libmvn_d_la_CXXFLAGS = -O2 -g

libmvn_d_la_SOURCES = $(COMMON_SOURCES)

libmvn_d_la_LIBADD = \
	verilog/libmvn_verilog_d.la

libmvn_d_la_LDFLAGS =


############################################################
## libmvn_p.la 用の変数の定義
############################################################

libmvn_p_la_CXXFLAGS = -O2 -pg

libmvn_p_la_SOURCES = $(COMMON_SOURCES)

libmvn_p_la_LIBADD = \
	verilog/libmvn_verilog_p.la

libmvn_p_la_LDFLAGS =
