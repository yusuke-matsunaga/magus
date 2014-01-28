## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1293 2008-02-21 02:25:52Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


noinst_LTLIBRARIES = libym_minisat.la


libym_minisat_la_SOURCES = \
	SatSolverMiniSat.h \
	SatSolverMiniSat.cc \
	Global.h \
	Heap.h \
	Solver.C \
	Solver.h \
	SolverTypes.h \
	Sort.h \
	VarOrder.h

libym_minisat_la_LIBADD =

libym_minisat_la_LDFLAGS =
