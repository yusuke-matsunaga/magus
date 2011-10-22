## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_verilog \
	common \
	parser \
	elaborator \
	. \
	tests

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_verilog.la

libym_verilog_la_SOURCES =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_verilog_la_SOURCES = dummy.cc

libym_verilog_la_LIBADD = \
	common/libym_vl_common.la \
	parser/libym_vl_parser.la \
	elaborator/libym_vl_elab.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la

libym_verilog_la_LDFLAGS =
