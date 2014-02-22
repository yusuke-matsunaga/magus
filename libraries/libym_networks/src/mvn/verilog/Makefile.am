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
	libmvn_verilog.la \
	libmvn_verilog_d.la \
	libmvn_verilog_p.la


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
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	DeclHash.h \
	DeclHash.cc \
	DeclMap.h \
	DeclMap.cc \
	Driver.h \
	Env.h \
	Env.cc \
	EnvMerger.h \
	EnvMerger.cc \
	MapRec.h \
	MvnVerilogReader.cc \
	MvnVlMap.cc \
	ReaderImpl.h \
	ReaderImpl.cc \
	ReaderImpl_item.cc \
	ReaderImpl_stmt.cc \
	ReaderImpl_expr.cc \
	Xmask.h \
	Xmask.cc


############################################################
## libmvn_verilog.la 用の変数の定義
############################################################

libmvn_verilog_la_CXXFLAGS = -O2

libmvn_verilog_la_SOURCES = $(COMMON_SOURCES)

libmvn_verilog_la_LIBADD =

libmvn_verilog_la_LDFLAGS =


############################################################
## libmvn_verilog_d.la 用の変数の定義
############################################################

libmvn_verilog_d_la_CXXFLAGS = -O2 -g

libmvn_verilog_d_la_SOURCES = $(COMMON_SOURCES)

libmvn_verilog_d_la_LIBADD =

libmvn_verilog_d_la_LDFLAGS =


############################################################
## libmvn_verilog_p.la 用の変数の定義
############################################################

libmvn_verilog_p_la_CXXFLAGS = -O2 -pg

libmvn_verilog_p_la_SOURCES = $(COMMON_SOURCES)

libmvn_verilog_p_la_LIBADD =

libmvn_verilog_p_la_LDFLAGS =
