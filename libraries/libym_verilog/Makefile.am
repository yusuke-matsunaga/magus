## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_verilog \
	common \
	parser \
	elaborator \
	. \
	tests

INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libym_verilog/parser/include \
	-I$(top_srcdir)/libym_verilog/elaborator/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libym_verilog.la

libym_verilog_la_SOURCES = \
	VlMgr.cc

libym_verilog_la_LIBADD = \
	common/libym_vl_common.la \
	parser/libym_vl_parser.la \
	elaborator/libym_vl_elab.la

libym_verilog_la_LDFLAGS =
