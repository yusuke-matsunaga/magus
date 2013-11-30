## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 697 2007-05-28 17:10:53Z matsunaga $

INCLUDES = \
	-I $(YMTOOLS_SRCDIR)/include \
	-I $(YMTOOLS_BUILDDIR)/include \
	-I $(top_srcdir)/src/bb/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libbb_pt.la

libbb_pt_la_SOURCES = \
	Assign.cc \
	Eq.h \
	Lt.h \
	Le.h \
	Gt.h \
	Ge.h \
	Constr.cc \
	PtMgr.cc \
	PtNode.cc \
	PtIdNode.h \
	PtIdNode.cc \
	PtConstNode.h \
	PtConstNode.cc \
	PtUnaryOp.h \
	PtUnaryOp.cc \
	PtNegOp.h \
	PtNegOp.cc \
	PtUminusOp.h \
	PtUminusOp.cc \
	PtBinaryOp.h \
	PtBinaryOp.cc \
	PtAddOp.h \
	PtAddOp.cc \
	PtAddOp.h \
	PtAddOp.cc \
	PtSubOp.h \
	PtSubOp.cc \
	PtMulOp.h \
	PtMulOp.cc \
	PtDivOp.h \
	PtDivOp.cc \
	PtModOp.h \
	PtModOp.cc \
	PtSllOp.h \
	PtSllOp.cc \
	PtSrlOp.h \
	PtSrlOp.cc \
	PtAndOp.h \
	PtAndOp.cc \
	PtOrOp.h \
	PtOrOp.cc \
	PtXorOp.h \
	PtXorOp.cc


libbb_pt_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
