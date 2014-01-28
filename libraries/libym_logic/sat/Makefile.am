## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = \
	ymsat \
	MiniSat \
	dimacs \
	.

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/sat/include


noinst_LTLIBRARIES = libym_logic_sat.la


libym_logic_sat_la_SOURCES = \
	SatSolver.cc \
	SatMsgHandlerImpl1.cc

libym_logic_sat_la_LIBADD = \
	ymsat/libym_sat_ymsat.la \
	MiniSat/libym_minisat.la \
	dimacs/libym_sat_dimacs.la

libym_logic_sat_la_LDFLAGS =
