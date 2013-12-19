## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_PROGRAMS = \
	rec \
	recordcut

# rec
rec_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES)

rec_SOURCES = \
	FuncHash.h \
	FuncHash.cc \
	FuncMgr.h \
	FuncMgr.cc \
	FuncRec.h \
	FuncRec.cc \
	GbmMgr.h \
	GbmMgr.cc \
	GbmNode.h \
	GbmNodeImpl.h \
	GbmNodeImpl.cc \
	GbmVar.h \
	GbmEngine.h \
	GbmEngine.cc \
	GbmSolver.h \
	GbmNaive.h \
	GbmNaive.cc \
	RecCmd.h \
	RecCmd.cc \
	tclAppInit.cc

rec_LDADD = \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

rec_LDFLAGS =

# recordcut
recordcut_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	@POPT_INCLUDES@

recordcut_SOURCES = \
	recordcut.cc \
	Cut.h \
	CutHash.h \
	CutHash.cc \
	CutMgr.h \
	CutMgr.cc \
	FuncMgr.h \
	FuncMgr.cc \
	FuncRec.h \
	FuncRec.cc

recordcut_LDADD = \
	$(top_builddir)/libec/libec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

recordcut_LDFLAGS =
