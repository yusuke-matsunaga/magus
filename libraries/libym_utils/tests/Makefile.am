## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS = \
	. \
	$(YM_EXTRA_TESTS_SUBDIRS)

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
	IODO_test \
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
	CompIDO_test \
	CompODO_test \
	StrListIDO_test

if YM_HAVE_ZLIB
noinst_PROGRAMS += \
	GzIDO_test \
	GzODO_test \
	zstream_test
endif


############################################################
## Common Variables
############################################################

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YMUTILS = ../libym_utils.la


############################################################
## Variables for building IODO_test
############################################################

IODO_test_SOURCES = \
	IODO_test.cc

IODO_test_LDADD = \
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
## Variables for building CompIDO_test
############################################################

CompIDO_test_SOURCES = \
	CompIDO_test.cc

CompIDO_test_LDFLAGS =

CompIDO_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building CompODO_test
############################################################

CompODO_test_SOURCES = \
	CompODO_test.cc

CompODO_test_LDFLAGS =

CompODO_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building StrListIDO_test
############################################################

StrListIDO_test_SOURCES = \
	StrListIDO_test.cc

StrListIDO_test_LDFLAGS =

StrListIDO_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building GzIDO_test
############################################################

GzIDO_test_SOURCES = \
	GzIDO_test.cc

GzIDO_test_LDFLAGS =

GzIDO_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building GzODO_test
############################################################

GzODO_test_SOURCES = \
	GzODO_test.cc

GzODO_test_LDFLAGS =

GzODO_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building zstream_test
############################################################

zstream_test_SOURCES = \
	zstream_test.cc

zstream_test_LDFLAGS =

zstream_test_LDADD = \
	$(YMUTILS)
