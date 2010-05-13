## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_logbase_tcl.la

libmagus_logbase_tcl_la_LIBADD = \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBYM_BDN) \
	$(YM_LIBS_LIBYM_BNET)
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
