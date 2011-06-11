## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = \
	ym_sat \
	include \
	ymsat \
	MiniSat \
	dimacs \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_sat.la

libym_sat_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libym_sat_la_SOURCES = \
	SatSolver.cc

libym_sat_la_LIBADD = \
	ymsat/libym_sat_ymsat.la \
	MiniSat/libym_minisat.la \
	dimacs/libym_sat_dimacs.la

#	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_sat_la_LDFLAGS =
