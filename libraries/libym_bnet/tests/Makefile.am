## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(top_srcdir)/include

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

LIBYM_BNET = $(top_builddir)/libraries/libym_bnet/libym_bnet.la

bin_PROGRAMS = \
	makenode \
	bliftest \
	iscas89test

makenode_SOURCES = \
	makenode.cc
makenode_LDADD = \
	$(LIBYM_BNET)

bliftest_SOURCES = \
	bliftest.cc
bliftest_LDADD = \
	$(LIBYM_BNET)

iscas89test_SOURCES = \
	iscas89test.cc
iscas89test_LDADD = \
	$(LIBYM_BNET)
