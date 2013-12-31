## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS = \
	. \
	$(YM_TESTS_SUBDIRS)

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
	ZCoder_test \
	ZDecoder_test \
	StrListIDO_test \
	Codec_test \
	BtgMatch_test \
	FlowTest

if YM_HAVE_ZLIB
noinst_PROGRAMS += \
	GzCoder_test \
	GzDecoder_test \
	zstream_test
endif

if YM_HAVE_BZLIB
noinst_PROGRAMS += \
	BzEngine_test \
	BzCoder_test \
	BzDecoder_test
endif

if YM_HAVE_LZMA
noinst_PROGRAMS += \
	LzmaEngine_test
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
## Variables for building ZCoder_test
############################################################

ZCoder_test_SOURCES = \
	ZCoder_test.cc

ZCoder_test_LDFLAGS =

ZCoder_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building ZDecoder_test
############################################################

ZDecoder_test_SOURCES = \
	ZDecoder_test.cc

ZDecoder_test_LDFLAGS =

ZDecoder_test_LDADD = \
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
## Variables for building Codec_test
############################################################

Codec_test_SOURCES = \
	Codec_test.cc

Codec_test_LDFLAGS =

Codec_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building GzCoder_test
############################################################

GzCoder_test_SOURCES = \
	GzCoder_test.cc

GzCoder_test_LDFLAGS =

GzCoder_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building GzDecoder_test
############################################################

GzDecoder_test_SOURCES = \
	GzDecoder_test.cc

GzDecoder_test_LDFLAGS =

GzDecoder_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building zstream_test
############################################################

zstream_test_SOURCES = \
	zstream_test.cc

zstream_test_LDFLAGS =

zstream_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building BzEngine_test
############################################################

BzEngine_test_SOURCES = \
	BzEngine_test.cc

BzEngine_test_LDFLAGS =

BzEngine_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building BzCoder_test
############################################################

BzCoder_test_SOURCES = \
	BzCoder_test.cc

BzCoder_test_LDFLAGS =

BzCoder_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building BzDecoder_test
############################################################

BzDecoder_test_SOURCES = \
	BzDecoder_test.cc

BzDecoder_test_LDFLAGS =

BzDecoder_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building LzmaEngine_test
############################################################

LzmaEngine_test_SOURCES = \
	LzmaEngine_test.cc

LzmaEngine_test_LDFLAGS =

LzmaEngine_test_LDADD = \
	$(YMUTILS)


############################################################
## Variables for building BtgMatch_test
############################################################

BtgMatch_test_SOURCES = \
	BtgMatch_test.cc

BtgMatch_test_LDFLAGS =

BtgMatch_test_LDADD = \
	$(YMUTILS)

############################################################
## Variables for building FlowTest
############################################################

FlowTest_SOURCES = \
	FlowTest.cc \
	FlowNode.h \
	FlowEdge.h \
	FlowNodeHeap.h \
	FlowNodeHeap.cc \
	FlowGraph.h \
	FlowGraph.cc

FlowTest_LDFLAGS =

FlowTest_LDADD = \
	$(YMUTILS)
