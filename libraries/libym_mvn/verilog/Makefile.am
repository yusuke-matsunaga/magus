## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvn_verilog.la

libym_mvn_verilog_la_SOURCES = \
	DeclHash.h \
	DeclHash.cc \
	DeclMap.h \
	DeclMap.cc \
	Driver.h \
	MapRec.h \
	MvnVerilogReader.cc \
	MvnVlMap.cc \
	ReaderImpl.h \
	ReaderImpl.cc \
	ReaderImpl_item.cc \
	ReaderImpl_stmt.cc \
	ReaderImpl_expr.cc \
	Env.h \
	Env.cc

libym_mvn_verilog_la_LDFLAGS =

libym_mvn_verilog_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

