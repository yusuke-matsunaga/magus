## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS = \
	. \
	$(YM_EXTRA_SUBDIRS)

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	file.test \
	namemgr.test \
	combigen.test \
	permgen.test \
	multicombigen.test \
	multipermgen.test \
	strbuff.test \
	dllist.test \
	heaptree.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)


############################################################
## Variables for building targets
############################################################

noinst_PROGRAMS = \
	binio_test \
	file_test \
	namemgr_test \
	combigen_test \
	permgen_test \
	multicombigen_test \
	multipermgen_test \
	strbuff_test \
	randgen_test \
	randpermgen_test \
	randcombigen_test \
	dllist_test \
	heaptree_test \
	mspg_test \
	mscg_test \
	z_test


############################################################
## Common Variables
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YMUTILS = ../libym_utils.la


############################################################
## Variables for building binio_test
############################################################

binio_test_SOURCES = \
	binio_test.cc

binio_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building file_test
############################################################

file_test_SOURCES = \
	file_test.cc

file_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building namemgr_test
############################################################

namemgr_test_SOURCES = \
	namemgr_test.cc

namemgr_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building combigen_test
############################################################

combigen_test_SOURCES = \
	combigen_test.cc

combigen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building permgen_test
############################################################

permgen_test_SOURCES = \
	permgen_test.cc

permgen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building multicombigen_test
############################################################

multicombigen_test_SOURCES = \
	multicombigen_test.cc

multicombigen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building multipermgen_test
############################################################

multipermgen_test_SOURCES = \
	multipermgen_test.cc

multipermgen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building strbuff_test
############################################################

strbuff_test_SOURCES = \
	strbuff_test.cc

strbuff_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building randgen_test
############################################################

randgen_test_SOURCES = \
	randgen_test.cc

randgen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building randpermgen_test
############################################################

randpermgen_test_SOURCES = \
	randpermgen_test.cc

randpermgen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building randcombigen_test
############################################################

randcombigen_test_SOURCES = \
	randcombigen_test.cc

randcombigen_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building dllist_test
############################################################

dllist_test_SOURCES = \
	dllist_test.cc

dllist_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building heaptree_test
############################################################

heaptree_test_SOURCES = \
	heaptree_test.cc
heaptree_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building mscg_test
############################################################

mscg_test_SOURCES = \
	mscg_test.cc

mscg_test_LDFLAGS =

mscg_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building mspg_test
############################################################

mspg_test_SOURCES = \
	mspg_test.cc

mspg_test_LDFLAGS =

mspg_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building z_test
############################################################

z_test_SOURCES = \
	z_test.cc

z_test_LDFLAGS =

z_test_LDADD = \
	$(YMUTILS)
