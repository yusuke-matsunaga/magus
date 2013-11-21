## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_YMSH = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_ymsh/libym_ymsh.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

noinst_PROGRAMS = \
	ymsh_test

ymsh_test_SOURCES = \
	ymsh_test.cc
ymsh_test_LDADD = \
	$(LIBYM_YMSH)
