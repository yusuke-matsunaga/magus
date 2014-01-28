## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_core_tcl.la

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libmagus_core_tcl_la_SOURCES = \
	MagMgr.cc \
	NetHandle.cc \
	BNetHandle.h \
	BNetHandle.cc \
	BdnHandle.h \
	BdnHandle.cc \
	MvnHandle.h \
	MvnHandle.cc \
	MagCmd.cc \
	NetCmd.cc \
	NewBNetwork.h \
	NewBNetwork.cc \
	NewBdn.h \
	NewBdn.cc \
	NewMvn.h \
	NewMvn.cc \
	ClrNetwork.h \
	ClrNetwork.cc \
	DelNetwork.h \
	DelNetwork.cc \
	CopyNetwork.h \
	CopyNetwork.cc \
	ChgNetwork.h \
	ChgNetwork.cc \
	PushNetwork.h \
	PushNetwork.cc \
	PopNetwork.h \
	PopNetwork.cc \
	CurNetwork.h \
	CurNetwork.cc \
	ListNetwork.h \
	ListNetwork.cc \
	ForNetwork.h \
	ForNetwork.cc \
	ReadBlif.h \
	ReadBlif.cc \
	ReadIscas89.h \
	ReadIscas89.cc \
	WriteBlif.h \
	WriteBlif.cc \
	WriteVerilog.h \
	WriteVerilog.cc \
	ReadLibrary.h \
	ReadLibrary.cc \
	ReadMislib.h \
	ReadMislib.cc \
	ReadLiberty.h \
	ReadLiberty.cc \
	RestoreLibrary.h \
	RestoreLibrary.cc \
	DumpLibrary.h \
	DumpLibrary.cc \
	core_init.cc

libmagus_core_tcl_la_LIBADD =

libmagus_core_tcl_la_LDFLAGS =
