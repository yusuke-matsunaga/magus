## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include \
	-I$(top_srcdir)/programs/magus/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_logbase_tcl.la

libmagus_logbase_tcl_la_LIBADD = \
	$(top_builddir)/libraries/libym_tclpp/libym_tclpp.la \
	$(top_builddir)/libraries/libym_bdn/libym_bdn.la \
	$(top_builddir)/libraries/libym_bnet/libym_bnet.la
libmagus_logbase_tcl_la_LDFLAGS = 
libmagus_logbase_tcl_la_SOURCES = \
	NetMgr.cc \
	NetHandle.cc \
	BaseCmd.cc \
	MagBNetCmd.cc \
	MagBdnCmd.cc \
	NodeCmd.h \
	NodeCmd.cc \
	NtwkBaseCmd.h \
	NtwkBaseCmd.cc \
	NtwkInfoCmd.h \
	NtwkInfoCmd.cc \
	NtwkIoCmd.h \
	NtwkIoCmd.cc \
	NtwkNdCmd.h \
	NtwkNdCmd.cc \
	ElimCmd.h \
	ElimCmd.cc \
	logbase_init.cc
