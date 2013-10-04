## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_smtlibv2 \
	include \
	parser \
	main \
	shell \
	. \
	tests

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_smtlibv2.la

libym_smtlibv2_la_SOURCES =

libym_smtlibv2_la_LIBADD = \
	parser/libym_smt_parser.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_smtlibv2_la_LDFLAGS =
