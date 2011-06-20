## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include

YFLAGS=-d -v

CLEANFILES = \
	verilog_grammer.cc verilog_grammer.h y.output verilog_grammer.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_parser_main.la


############################################################
## libym_vl_parser_main.la
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
