## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_imp.la

libym_imp_la_CPPFLAGS = \
	-I$(top_srcdir)/imp/include \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@GMP_INCLUDES@

libym_imp_la_SOURCES = \
	BNodeMap.cc \
	ImpInfo.cc \
	ImpHash.cc \
	ImpCell.h \
	ImpBase.cc \
	ImpMgr.cc \
	ImpNode.cc \
	ImpInput.h \
	ImpInput.cc \
	ImpAnd.h \
	ImpAnd.cc \
	ImpListRec.h \
	ImpListRec.cc \
	ImpListRec2.h \
	ImpListRec2.cc \
	ImpListRec3.h \
	ImpListRec3.cc \
	StrImp.h \
	StrImp.cc \
	ContraImp.h \
	ContraImp.cc \
	CnfImp.h \
	CnfImp.cc \
	CnfClause.h \
	CnfImp2.h \
	CnfImp2.cc \
	SatImp.h \
	SatImp.cc \
	ConstImp.h \
	ConstImp.cc \
	RlImp.h \
	RlImp.cc \
	NaImp.h \
	NaImp.cc \
	ImpValList.h \
	ImpValList.cc
