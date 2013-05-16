## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $


SUBDIRS = \
	ym_utils \
	$(YM_EXTRA_SUBDIRS) \
	. \
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
	BinIO.cc \
	Binder.cc \
	CombiGen.cc \
	DlList.cc \
	File.cc \
	FileInfo.h \
	FileInfo.cc \
	FileInfoMgr.h \
	FileInfoMgr.cc \
	FileScanner.cc \
	FragAlloc.cc \
	GenBase.cc \
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
	PermGen.cc \
	PoptMainApp.cc \
	RandGen.cc \
	RefCount.cc \
	SimpleAlloc.cc \
	StrBuff.cc \
	StrPool.h \
	StrPool.cc \
	StopWatch.cc \
	UnitAlloc.cc \
	random.cc

libym_utils_la_LIBADD = \
	$(YM_EXTRA_LIBADD) \
	@POPT_LIBS@

libym_utils_la_LDFLAGS =
