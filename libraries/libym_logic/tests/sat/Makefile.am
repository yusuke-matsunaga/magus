## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1293 2008-02-21 02:25:52Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

LIBYM_SAT = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


noinst_PROGRAMS = \
	dimacstest

bin_PROGRAMS = \
	ymsat \
	ymsat2


dimacstest_SOURCES = \
	dimacstest.cc

dimacstest_LDADD = \
	$(LIBYM_SAT)

ymsat_SOURCES = \
	ymsat.cc

ymsat_LDADD = \
	$(LIBYM_SAT)

ymsat2_SOURCES = \
	ymsat2.cc

ymsat2_LDADD = \
	$(LIBYM_SAT)
