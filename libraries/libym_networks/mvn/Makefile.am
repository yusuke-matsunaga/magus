## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS = \
	verilog \
	.

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvn.la

libym_mvn_la_LDFLAGS =

libym_mvn_la_LIBADD = \
	verilog/libym_mvn_verilog.la

libym_mvn_la_SOURCES = \
	MvnConst.h \
	MvnConst.cc \
	MvnConstBitSelect.h \
	MvnConstBitSelect.cc \
	MvnConstPartSelect.h \
	MvnConstPartSelect.cc \
	MvnDff.h \
	MvnDff.cc \
	MvnDumper.cc \
	MvnMgr.cc \
	MvnModule.cc \
	MvnNode.cc \
	MvnNodeBase.h \
	MvnNodeBase.cc \
	MvnPort.cc \
	MvnVerilogWriter.cc

