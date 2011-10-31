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
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

lsim_SOURCES = \
	lsim.cc \
	Lsim.h \
	LsimNaive.h \
	LsimNaive.cc \
	LsimNaive2.h \
	LsimNaive2.cc \
	LsimBdd.h \
	LsimBdd.cc \
	LsimBdd2.h \
	LsimBdd2.cc \
	LsimBdd1.h \
	LsimBdd1.cc \
	LsimBdd3.h \
	LsimBdd3.cc \
	LsimBdd10.h \
	LsimBdd10.cc \
	LsimLcc.h \
	LsimLcc.cc \
	LsimMpx.h \
	LsimMpx.cc \
	LsimMpx2.h \
	LsimMpx2.cc

lsim_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

lsim_LDFLAGS =
