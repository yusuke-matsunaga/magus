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
	igf

igf_SOURCES = \
	igf.cc \
	IguGen.h \
	IguGen.cc \
	RvMgr.cc \
	VectSetList.h \
	VectSetList.cc \
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
