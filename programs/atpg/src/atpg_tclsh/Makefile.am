## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

AM_CXXFLAGS = \
	-DATPG_LIBDIR='"$(ATPG_LIBDIR)"' \
	-DATPG_DATADIR='"$(ATPG_DATADIR)"' \
	-DATPG_BINDIR='"$(ATPG_BINDIR)"'

INCLUDES = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMLIBS_BUILDDIR) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	atpg_tclsh

atpg_tclsh_SOURCES = \
	tclAppInit.cc \
	atpg_init.cc \
	AtpgCmd.h \
	AtpgCmd.cc \
	AtpgMsgHandler.h \
	AtpgMsgHandler.cc \
	NtwkIoCmd.h \
	NtwkIoCmd.cc \
	FaultCmd.h \
	FaultCmd.cc \
	PrintFaultCmd.h \
	PrintFaultCmd.cc \
	PrintPatCmd.h \
	PrintPatCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	RfsimCmd.h \
	RfsimCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc

atpg_tclsh_LDADD = \
	$(top_builddir)/src/main/libatpg_main.la \
	$(top_builddir)/src/dtpg_classic/libatpg_dtpg_cl.la \
	$(top_builddir)/src/dtpg_sat/libatpg_dtpg_sat.la \
	$(top_builddir)/src/fsim/libatpg_fsim.la \
	$(top_builddir)/src/fsimx/libatpg_fsimx.la \
	$(top_builddir)/src/base/libatpg_base.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)
