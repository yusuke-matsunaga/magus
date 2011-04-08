## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS = \
	ym_mvn \
	verilog \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvn.la

libym_mvn_la_SOURCES = \
	dump.cc \
	MvnArithOp.h \
	MvnArithOp.cc \
	MvnBinaryOp.h \
	MvnBinaryOp.cc \
	MvnBitSelect.h \
	MvnBitSelect.cc \
	MvnCmpl.h \
	MvnCmpl.cc \
	MvnConcat.h \
	MvnConcat.cc \
	MvnConst.h \
	MvnConst.cc \
	MvnConstBitSelect.h \
	MvnConstBitSelect.cc \
	MvnConstPartSelect.h \
	MvnConstPartSelect.cc \
	MvnDff.h \
	MvnDff.cc \
	MvnInput.h \
	MvnInput.cc \
	MvnInout.h \
	MvnInout.cc \
	MvnIte.h \
	MvnIte.cc \
	MvnLogOp.h \
	MvnLogOp.cc \
	MvnMgr.cc \
	MvnModule.cc \
	MvnNode.cc \
	MvnNot.h \
	MvnNot.cc \
	MvnOutput.h \
	MvnOutput.cc \
	MvnPartSelect.h \
	MvnPartSelect.cc \
	MvnPort.cc \
	MvnRelOp.h \
	MvnRelOp.cc \
	MvnRop.h \
	MvnRop.cc \
	MvnTernaryOp.h \
	MvnTernaryOp.cc \
	MvnThrough.h \
	MvnThrough.cc \
	MvnUdp.h \
	MvnUdp.cc \
	MvnUnaryOp.h \
	MvnUnaryOp.cc \
	MvnVerilogWriter.cc

libym_mvn_la_LDFLAGS =

libym_mvn_la_LIBADD = \
	verilog/libym_mvn_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

