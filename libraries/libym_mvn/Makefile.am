## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS = \
	ym_mvn \
	. \
	verilog \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvn.la

libym_mvn_la_SOURCES = \
	MvArithOp.h \
	MvArithOp.cc \
	MvBinaryOp.h \
	MvBinaryOp.cc \
	MvConcat.h \
	MvConcat.cc \
	MvConst.h \
	MvConst.cc \
	MvInput.h \
	MvInput.cc \
	MvInst.h \
	MvInst.cc \
	MvIte.h \
	MvIte.cc \
	MvLogOp.h \
	MvLogOp.cc \
	MvMgr.cc \
	MvModule.cc \
	MvNode.cc \
	MvNot.h \
	MvNot.cc \
	MvOutput.h \
	MvOutput.cc \
	MvPort.cc \
	MvRelOp.h \
	MvRelOp.cc \
	MvRop.h \
	MvRop.cc \
	MvTernaryOp.h \
	MvTernaryOp.cc \
	MvThrough.h \
	MvThrough.cc \
	MvUnaryOp.h \
	MvUnaryOp.cc

libym_mvn_la_LDFLAGS =

libym_mvn_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

