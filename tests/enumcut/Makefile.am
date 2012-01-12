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
	lr

enumcut_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@

enumcut_SOURCES = \
	enumcut.cc \
	EnumCut.h \
	EnumCut.cc \
	BottomUp.h \
	BottomUp.cc \
	TopDown.h \
	TopDown.cc \
	ZddImp.h \
	ZddImp.cc

enumcut_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

enumcut_LDFLAGS =

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
	BottomUp.h \
	BottomUp.cc \
	TopDown.h \
	TopDown.cc \
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
