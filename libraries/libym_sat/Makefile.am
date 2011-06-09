## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = \
	ym_sat \
	analyzer \
	MiniSat \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_sat.la

libym_sat_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_sat_la_SOURCES = \
	SatReason.h \
	Watcher.h \
	SatClause.h \
	SatClause.cc \
	AssignList.h \
	AssignList.cc \
	SatAnalyzer.h \
	SatSolverImpl.h \
	SatSolverImpl.cc \
	SatSolverImplR.h \
	SatSolverImplR.cc \
	SatSolver.cc \
	SatSolverFactory.cc \
	DimacsParser.cc \
	DimacsParserImpl.h \
	DimacsScanner.h \
	DimacsScanner.cc

libym_sat_la_LIBADD = \
	MiniSat/libym_minisat.la \
	analyzer/libym_satanalyzer.la

#	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_sat_la_LDFLAGS =
