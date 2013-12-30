## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $


SUBDIRS = \
	ym_utils \
	. \
	$(YM_EXTRA_SUBDIRS) \
	tests

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_utils.la


############################################################
## Variables for building libym_utils.la
############################################################

libym_utils_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

libym_utils_la_SOURCES = \
	Alloc.cc \
	Binder.cc \
	BtgEdge.h \
	BtgHeapTree.h \
	BtgHeapTree.cc \
	BtgMatch.cc \
	BtgNode.h \
	CombiGen.cc \
	ZCoder.h \
	ZCoder.cc \
	ZDecoder.h \
	ZDecoder.cc \
	DlList.cc \
	File.cc \
	FileBuff.h \
	FileBuff.cc \
	FileCoder.cc \
	FileDecoder.cc \
	FileIDO.cc \
	FileODO.cc \
	FileInfo.h \
	FileInfo.cc \
	FileInfoMgr.h \
	FileInfoMgr.cc \
	FragAlloc.cc \
	GenBase.cc \
	IDO.cc \
	ItvlCell.h \
	ItvlMgrImpl.h \
	ItvlMgrImpl.cc \
	ItvlMgr.cc \
	MFSet.cc \
	MsgHandler.cc \
	MsgMgr.cc \
	MsgMgrImpl.h \
	MsgMgrImpl.cc \
	MultiCombiGen.cc \
	MultiGenBase.cc \
	MultiSetCombiGen.cc \
	MultiSetGenBase.cc \
	MultiSetPermGen.cc \
	MultiPermGen.cc \
	NameMgr.cc \
	ODO.cc \
	PermGen.cc \
	PoptMainApp.cc \
	RandGen.cc \
	RefCount.cc \
	Scanner.cc \
	SimpleAlloc.cc \
	StrBuff.cc \
	StrPool.h \
	StrPool.cc \
	StopWatch.cc \
	ThruCoder.h \
	ThruCoder.cc \
	ThruDecoder.h \
	ThruDecoder.cc \
	UnitAlloc.cc \
	random.cc

libym_utils_la_LIBADD = \
	@POPT_LIBS@

libym_utils_la_LDFLAGS =


if YM_HAVE_ZLIB
libym_utils_la_SOURCES += \
	zstream.cc \
	GzCoder.h \
	GzCoder.cc \
	GzDecoder.h \
	GzDecoder.cc

libym_utils_la_LDFLAGS += \
	-lz
endif

if YM_HAVE_BZLIB
libym_utils_la_SOURCES += \
	BzCoder.h \
	BzCoder.cc \
	BzDecoder.h \
	BzDecoder.cc

libym_utils_la_LDFLAGS += \
	-lbz2
endif

if YM_HAVE_LZMA
#libym_utils_la_SOURCES +=

libym_utils_la_LDFLAGS += \
	-llzma
endif
