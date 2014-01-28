## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = \
	smtlibv2 \
	core \
	.

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


noinst_LTLIBRARIES = libym_logic_smt.la


libym_logic_smt_la_SOURCES =

libym_logic_smt_la_LIBADD = \
	core/libym_logic_smt_core.la \
	smtlibv2/libym_logic_smt_smtlibv2.la

libym_logic_smt_la_LDFLAGS =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_logic_smt_la_SOURCES = dummy.cc
