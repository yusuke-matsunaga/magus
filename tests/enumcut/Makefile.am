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
	init_table \
	enumcut \
	rec \
	recordcut \
	lr


# enumcut
enumcut_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

enumcut_SOURCES = \
	enumcut.cc \
	EnumCut.h \
	EnumCut.cc \
	EnumCut2.h \
	EnumCut2.cc \
	BottomUp.h \
	BottomUp.cc \
	TopDown.h \
	TopDown.cc \
	ZddImp.h \
	ZddImp.cc \
	ZddImp2.h \
	ZddImp2.cc \
	ZddTopDown.h \
	ZddTopDown.cc

enumcut_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

enumcut_LDFLAGS =

# rec
rec_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES)

rec_SOURCES = \
	EnumCut.h \
	EnumCut.cc \
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
	TopDown.h \
	TopDown.cc \
	RecCmd.h \
	RecCmd.cc \
	tclAppInit.cc

rec_LDADD = \
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
	@POPT_INCLUDES@

recordcut_SOURCES = \
	recordcut.cc \
	Cut.h \
	CutHash.h \
	CutHash.cc \
	CutMgr.h \
	CutMgr.cc \
	EnumCut.h \
	EnumCut.cc \
	FuncMgr.h \
	FuncMgr.cc \
	FuncRec.h \
	FuncRec.cc \
	TopDown.h \
	TopDown.cc

recordcut_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

recordcut_LDFLAGS =

# lr
lr_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

lr_SOURCES = \
	RwtNode.h \
	RwtPat.h \
	RwtPatGen.h \
	RwtPatGen.cc \
	RwtPatList.h \
	RwtMgr.h \
	RwtMgr.cc \
	set_abc_table.cc \
	EnumCut.h \
	EnumCut.cc \
	TopDown2.h \
	TopDown2.cc \
	RwtOp.h \
	RwtOp.cc \
	lr.cc

lr_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

lr_LDFLAGS =


init_table_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

init_table_SOURCES = \
	init_table.cc \
	RwtNode.h \
	RwtPat.h \
	RwtPatGen.h \
	RwtPatGen.cc \
	RwtPatList.h \
	RwtMgr.h \
	RwtMgr.cc \
	set_abc_table.cc

init_table_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

init_table_LDFLAGS =
