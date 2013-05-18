## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

AM_CXXFLAGS = \
	-DATPG_LIBDIR='"$(ATPG_LIBDIR)"' \
	-DATPG_DATADIR='"$(ATPG_DATADIR)"' \
	-DATPG_BINDIR='"$(ATPG_BINDIR)"'

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	@POPT_INCLUDES@

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	igf \
	phf \
	phf2 \
	phf3

igf_SOURCES = \
	igf.cc \
	IguGen.h \
	IguGen.cc \
	RvMgr.cc \
	VectSetList.h \
	Variable.h \
	Variable.cc

igf_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD) \
	-lrt \
	@POPT_LIBS@

phf_SOURCES = \
	phf.cc \
	RvMgr.cc \
	PhfNode.h \
	PhfEdge.h \
	PhfGen.h \
	PhfGen.cc \
	InputFunc.h \
	VarFunc.h \
	VarFunc.cc \
	IguGen.h \
	IguGen.cc \
	Variable.h \
	Variable.cc

phf_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD) \
	-lrt \
	@POPT_LIBS@

phf2_SOURCES = \
	phf2.cc \
	RvMgr.cc \
	Phf2Node.h \
	Phf2Edge.h \
	Phf2Gen.h \
	Phf2Gen.cc \
	InputFunc.h \
	VarFunc.h \
	VarFunc.cc \
	IguGen.h \
	IguGen.cc \
	Variable.h \
	Variable.cc

phf2_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD) \
	-lrt \
	@POPT_LIBS@

phf3_SOURCES = \
	phf3.cc \
	RvMgr.cc \
	Phf3Node.h \
	Phf3Edge.h \
	Phf3Gen.h \
	Phf3Gen.cc \
	InputFunc.h \
	VarFunc.h \
	VarFunc.cc \
	IguGen.h \
	IguGen.cc \
	Variable.h \
	Variable.cc

phf3_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD) \
	-lrt \
	@POPT_LIBS@
