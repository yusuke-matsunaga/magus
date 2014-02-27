## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES = \
	verilog_grammer.cc \
	verilog_grammer.h \
	y.output \
	verilog_grammer.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = \
	libvlparser_main.la \
	libvlparser_main_d.la \
	libvlparser_main_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_builddir)/../libym_utils/include \
	-I$(top_srcdir)/src/parser/include

AM_YFLAGS = -d -v


############################################################
## 今日の変数定義
############################################################

COMMON_SOURCES = \
	verilog_grammer.yy \
	Parser.cc \
	Parser_decl.cc \
	Parser_expr.cc \
	Parser_generate.cc \
	Parser_inst.cc \
	Parser_int.h \
	Parser_item.cc \
	Parser_misc.cc \
	Parser_module.cc \
	Parser_port.cc \
	Parser_stmt.cc \
	Parser_udp.cc \c
	PuHierName.h


############################################################
## libvlparser_main.la 用の変数の定義
############################################################

libvlparser_main_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libvlparser_main_la_SOURCES = $(COMMON_SOURCES)

libvlparser_main_la_LIBADD =

libvlparser_main_la_LDFLAGS =


############################################################
## libvlparser_main_d.la 用の変数の定義
############################################################

libvlparser_main_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libvlparser_main_d_la_SOURCES = $(COMMON_SOURCES)

libvlparser_main_d_la_LIBADD =

libvlparser_main_d_la_LDFLAGS =


############################################################
## libvlparser_main_p.la 用の変数の定義
############################################################

libvlparser_main_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libvlparser_main_p_la_SOURCES = $(COMMON_SOURCES)

libvlparser_main_p_la_LIBADD =

libvlparser_main_p_la_LDFLAGS =
