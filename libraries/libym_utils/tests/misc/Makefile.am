## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
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
	gctest \
	SimpleAllocTest

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
	LzmaEngine_test \
	LzmaCoder_test \
	LzmaDecoder_test
endif


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src


############################################################
## Common Variables
############################################################

YMUTILS = $(top_builddir)/src/libym_utils.la

YMUTILS_D = $(top_builddir)/src/libym_utils_d.la


############################################################
## Variables for building IODO_test
############################################################

IODO_test_SOURCES = \
	IODO_test.cc

IODO_test_LDADD = \
	$(YMUTILS)

IODO_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building file_test
############################################################

file_test_SOURCES = \
	file_test.cc

file_test_LDADD = \
	$(YMUTILS)

file_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building namemgr_test
############################################################

namemgr_test_SOURCES = \
	namemgr_test.cc

namemgr_test_LDADD = \
	$(YMUTILS)

namemgr_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building combigen_test
############################################################

combigen_test_SOURCES = \
	combigen_test.cc

combigen_test_LDADD = \
	$(YMUTILS)

combigen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building permgen_test
############################################################

permgen_test_SOURCES = \
	permgen_test.cc

permgen_test_LDADD = \
	$(YMUTILS)

permgen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building multicombigen_test
############################################################

multicombigen_test_SOURCES = \
	multicombigen_test.cc

multicombigen_test_LDADD = \
	$(YMUTILS)

multicombigen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building multipermgen_test
############################################################

multipermgen_test_SOURCES = \
	multipermgen_test.cc

multipermgen_test_LDADD = \
	$(YMUTILS)

multipermgen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building strbuff_test
############################################################

strbuff_test_SOURCES = \
	strbuff_test.cc

strbuff_test_LDADD = \
	$(YMUTILS)

strbuff_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building randgen_test
############################################################

randgen_test_SOURCES = \
	randgen_test.cc

randgen_test_LDADD = \
	$(YMUTILS)

randgen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building randpermgen_test
############################################################

randpermgen_test_SOURCES = \
	randpermgen_test.cc

randpermgen_test_LDADD = \
	$(YMUTILS)

randpermgen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building randcombigen_test
############################################################

randcombigen_test_SOURCES = \
	randcombigen_test.cc

randcombigen_test_LDADD = \
	$(YMUTILS)

randcombigen_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building dllist_test
############################################################

dllist_test_SOURCES = \
	dllist_test.cc

dllist_test_LDADD = \
	$(YMUTILS)

dllist_test_LDFLAGS = \
	$(AM_LDFLAS)


############################################################
## Variables for building heaptree_test
############################################################

heaptree_test_SOURCES = \
	heaptree_test.cc

heaptree_test_LDADD = \
	$(YMUTILS)

heaptree_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building mscg_test
############################################################

mscg_test_SOURCES = \
	mscg_test.cc

mscg_test_LDADD = \
	$(YMUTILS)

mscg_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building mspg_test
############################################################

mspg_test_SOURCES = \
	mspg_test.cc

mspg_test_LDADD = \
	$(YMUTILS)

mspg_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building CompIDO_test
############################################################

CompIDO_test_SOURCES = \
	CompIDO_test.cc

CompIDO_test_LDADD = \
	$(YMUTILS)

CompIDO_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building CompODO_test
############################################################

CompODO_test_SOURCES = \
	CompODO_test.cc

CompODO_test_LDADD = \
	$(YMUTILS)

CompODO_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building ZCoder_test
############################################################

ZCoder_test_SOURCES = \
	ZCoder_test.cc

ZCoder_test_LDADD = \
	$(YMUTILS)

ZCoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building ZDecoder_test
############################################################

ZDecoder_test_SOURCES = \
	ZDecoder_test.cc

ZDecoder_test_LDADD = \
	$(YMUTILS)

ZDecoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building StrListIDO_test
############################################################

StrListIDO_test_SOURCES = \
	StrListIDO_test.cc

StrListIDO_test_LDADD = \
	$(YMUTILS)

StrListIDO_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building Codec_test
############################################################

Codec_test_SOURCES = \
	Codec_test.cc

Codec_test_LDADD = \
	$(YMUTILS)

Codec_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building GzCoder_test
############################################################

GzCoder_test_SOURCES = \
	GzCoder_test.cc

GzCoder_test_LDADD = \
	$(YMUTILS)

GzCoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building GzDecoder_test
############################################################

GzDecoder_test_SOURCES = \
	GzDecoder_test.cc

GzDecoder_test_LDADD = \
	$(YMUTILS)

GzDecoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building zstream_test
############################################################

zstream_test_SOURCES = \
	zstream_test.cc

zstream_test_LDADD = \
	$(YMUTILS)

zstream_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building BzEngine_test
############################################################

BzEngine_test_SOURCES = \
	BzEngine_test.cc

BzEngine_test_LDADD = \
	$(YMUTILS)

BzEngine_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building BzCoder_test
############################################################

BzCoder_test_SOURCES = \
	BzCoder_test.cc

BzCoder_test_LDADD = \
	$(YMUTILS)

BzCoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building BzDecoder_test
############################################################

BzDecoder_test_SOURCES = \
	BzDecoder_test.cc

BzDecoder_test_LDADD = \
	$(YMUTILS)

BzDecoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building LzmaEngine_test
############################################################

LzmaEngine_test_SOURCES = \
	LzmaEngine_test.cc

LzmaEngine_test_LDADD = \
	$(YMUTILS)

LzmaEngine_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building LzmaCoder_test
############################################################

LzmaCoder_test_SOURCES = \
	LzmaCoder_test.cc

LzmaCoder_test_LDADD = \
	$(YMUTILS)

LzmaCoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building LzmaDecoder_test
############################################################

LzmaDecoder_test_SOURCES = \
	LzmaDecoder_test.cc

LzmaDecoder_test_LDADD = \
	$(YMUTILS)

LzmaDecoder_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building BtgMatch_test
############################################################

BtgMatch_test_SOURCES = \
	BtgMatch_test.cc

BtgMatch_test_LDADD = \
	$(YMUTILS)

BtgMatch_test_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building gctest
############################################################

gctest_SOURCES = \
	gctest.cc

gctest_LDADD = \
	$(YMUTILS)

gctest_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building SimpleAllocTest
############################################################

SimpleAllocTest_SOURCES = \
	SimpleAllocTest.cc

SimpleAllocTest_LDADD = \
	$(YMUTILS)

SimpleAllocTest_LDFLAGS = \
	$(AM_LDFLAGS)


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

FlowTest_LDADD = \
	$(YMUTILS)

FlowTest_LDFLAGS = \
	$(AM_LDFLAGS)
