## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	atpg_tclsh

AM_CXXFLAGS = \
	-DATPG_LIBDIR='"$(ATPG_LIBDIR)"' \
	-DATPG_DATADIR='"$(ATPG_DATADIR)"' \
	-DATPG_BINDIR='"$(ATPG_BINDIR)"'

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old/include

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
	PrintPatStatsCmd.h \
	PrintPatStatsCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	RfsimCmd.h \
	RfsimCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc \
	DtpgOldCmd.h \
	DtpgOldCmd.cc

atpg_tclsh_LDADD = \
	$(top_builddir)/old/src/main/libatpg_main.la \
	$(top_builddir)/old/src/dtpg/libatpg_dtpg.la \
	$(top_builddir)/old/src/dtpg.old/libatpg_dtpg_old.la \
	$(top_builddir)/old/src/fsim2/libatpg_fsim2.la \
	$(top_builddir)/old/src/fsim3/libatpg_fsim3.la \
	$(top_builddir)/old/src/fsimx/libatpg_fsimx.la \
	$(top_builddir)/old/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)
