## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =


noinst_PROGRAMS = \
	imp

imp_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

imp_SOURCES = \
	imp.cc \
	BNodeMap.h \
	BNodeMap.cc \
	ImpVal.h \
	ImpCell.h \
	ImpList.h \
	ImpInfo.h \
	ImpInfo.cc \
	ImpBase.h \
	ImpMgr.h \
	ImpMgr.cc \
	ImpNode.h \
	ImpNode.cc \
	ImpInput.h \
	ImpInput.cc \
	ImpAnd.h \
	ImpAnd.cc \
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
	RlImp.h \
	RlImp.cc \
	NaImp.h \
	NaImp.cc

imp_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

imp_LDFLAGS =
