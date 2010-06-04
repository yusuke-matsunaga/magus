## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS = \
	ym_ceq \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_ceq.la

INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)

libym_ceq_la_SOURCES = \
	check_ceq.cc

libym_ceq_la_LIBADD = \
	$(top_builddir)/libraries/libym_aig/libym_aig.la \
	$(top_builddir)/libraries/libym_sat/libym_sat.la \
	$(top_builddir)/libraries/libym_bnet/libym_bnet.la

libym_ceq_la_LDFLAGS =

