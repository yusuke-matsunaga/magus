## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1617 2008-07-27 16:56:12Z matsunaga $

SUBDIRS =

AM_CXXFLAGS = \
	-DMATPG_LIBDIR='"$(MATPG_LIBDIR)"' \
	-DMATPG_DATADIR='"$(MATPG_DATADIR)"' \
	-DMATPG_BINDIR='"$(MATPG_BINDIR)"'

INCLUDES = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/libmatpg

MAINTAINERCLEANFILES = Makefile.in


noinst_LTLIBRARIES = \
	libmatpg_tcl.la

bin_PROGRAMS = \
	matpg_tclsh


libmatpg_tcl_la_SOURCES = \
	MatpgMsgHandler.h \
	MatpgMsgHandler.cc \
	ReadBlifCmd.h \
	ReadBlifCmd.cc \
	ReadIscas89Cmd.h \
	ReadIscas89Cmd.cc \
	SetFaultCmd.h \
	SetFaultCmd.cc \
	PrintFaultCmd.h \
	PrintFaultCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	PrintPatCmd.h \
	PrintPatCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	RfsimCmd.h \
	RfsimCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc \
	matpg_init.cc

libmatpg_tcl_la_LIBADD = \
	$(YM_LIBS_LIBYM_TGNET) \
	$(YM_LIBS_LIBYM_LEXP) \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBYM_UTILS)


matpg_tclsh_SOURCES = \
	tclAppInit.cc

matpg_tclsh_LDADD = \
	libmatpg_tcl.la \
	$(top_builddir)/src/libmatpg/libmatpg.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LADD)
