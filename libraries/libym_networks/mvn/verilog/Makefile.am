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

noinst_LTLIBRARIES = libym_mvn_verilog.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## libym_mvn_verilog.la 用の変数の定義
############################################################

libym_mvn_verilog_la_SOURCES = \
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

libym_mvn_verilog_la_LIBADD =

libym_mvn_verilog_la_LDFLAGS =
