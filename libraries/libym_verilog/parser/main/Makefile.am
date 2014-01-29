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
	libym_vl_parser_main.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include

AM_YFLAGS = -d -v


############################################################
## libym_vl_parser_main.la 用の変数の定義
############################################################

libym_vl_parser_main_la_SOURCES = \
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

libym_vl_parser_main_la_LIBADD =

libym_vl_parser_main_la_LDFLAGS =
