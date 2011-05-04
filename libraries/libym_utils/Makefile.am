## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $


SUBDIRS = \
	. \
	ym_utils \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_utils.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_utils_la_SOURCES = \
	BinIO.cc \
	Binder.cc \
	DlList.cc \
	File.cc \
	FileInfoMgr.cc \
	FileScanner.cc \
	FragAlloc.cc \
	Generator.cc \
	ItvlCell.h \
	ItvlCell.cc \
	ItvlMgr.cc \
	MFSet.cc \
	MsgHandler.cc \
	MsgMgr.cc \
	MsgMgrImpl.h \
	MsgMgrImpl.cc \
	NameMgr.cc \
	RandGen.cc \
	RefCount.cc \
	SimpleAlloc.cc \
	StrBuff.cc \
	StrPool.h \
	StrPool.cc \
	StopWatch.cc \
	UnitAlloc.cc \
	random.cc

libym_utils_la_LIBADD =

libym_utils_la_LDFLAGS =

