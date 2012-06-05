## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

noinst_PROGRAMS = \
	npngen \
	cannonical \
	genaig

genaig_SOURCES = \
	genaig.cc \
	GenAig.h \
	GenAig.cc \
	GenAigNaive.h \
	GenAigNaive.cc \
	GenAigDj.h \
	GenAigDj.cc

genaig_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	@POPT_LIBS@

npngen_SOURCES = \
	npngen.cc

npngen_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

cannonical_SOURCES = \
	cannonical.cc

cannonical_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
