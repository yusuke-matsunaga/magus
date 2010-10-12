## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = \
#	read_blif.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_TECHMAP = $(YMTOOLS_BUILDDIR)/libraries/libym_techmap/libym_techmap.la
LIBYM_BNET = $(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

bin_PROGRAMS = \
	read_blif

read_blif_SOURCES = \
	read_blif.cc

read_blif_LDADD = \
	$(LIBYM_TECHMAP) \
	$(LIBYM_BNET)
