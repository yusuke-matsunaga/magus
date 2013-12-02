## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	parser \
	shell \
	.

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_logic_smt_smtlibv2.la

libym_logic_smt_smtlibv2_la_SOURCES =

libym_logic_smt_smtlibv2_la_LIBADD = \
	shell/libym_smt_shell.la \
	parser/libym_smt_parser.la

libym_logic_smt_smtlibv2_la_LDFLAGS = \
	-lreadline -lncurses
