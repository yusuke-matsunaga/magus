## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvnbdnconv.la

libym_mvnbdnconv_la_SOURCES = \
	MvNodeMap.cc \
	Mvn2Sbj.cc \
	ThroughConv.h \
	ThroughConv.cc \
	NotConv.h \
	NotConv.cc \
	AndConv.h \
	AndConv.cc \
	OrConv.h \
	OrConv.cc \
	XorConv.h \
	XorConv.cc \
	RandConv.h \
	RandConv.cc \
	RorConv.h \
	RorConv.cc \
	RxorConv.h \
	RxorConv.cc \
	CmplConv.h \
	CmplConv.cc \
	AddConv.h \
	AddConv.cc \
	SubConv.h \
	SubConv.cc \
	MultConv.h \
	MultConv.cc \
	DivConv.h \
	DivConv.cc \
	ModConv.h \
	ModConv.cc \
	PowConv.h \
	PowConv.cc \
	SllConv.h \
	SllConv.cc \
	SrlConv.h \
	SrlConv.cc \
	SlaConv.h \
	SlaConv.cc \
	SraConv.h \
	SraConv.cc \
	EqConv.h \
	EqConv.cc \
	LtConv.h \
	LtConv.cc \
	IteConv.h \
	IteConv.cc \
	ConcatConv.h \
	ConcatConv.cc \
	ConstBitSelectConv.h \
	ConstBitSelectConv.cc \
	ConstPartSelectConv.h \
	ConstPartSelectConv.cc

libym_mvnbdnconv_la_LDFLAGS =

libym_mvnbdnconv_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_mvn/libym_mvn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdn/libym_bdn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

