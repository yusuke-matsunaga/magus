## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =


noinst_PROGRAMS = \
	lsim

lsim_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

lsim_SOURCES = \
	lsim.cc \
	Lsim.h \
	LsimNaive.h \
	LsimNaive.cc

lsim_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

lsim_LDFLAGS =
