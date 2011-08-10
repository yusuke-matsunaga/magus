## Process this file with automake to produce Makefile.in

SUBDIRS = \
	verilog \
	.

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvn.la

libym_mvn_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_mvn_la_LDFLAGS =

libym_mvn_la_LIBADD = \
	verilog/libym_mvn_verilog.la

libym_mvn_la_SOURCES = \
	MvnCaseEq.h \
	MvnCaseEq.cc \
	MvnCell.h \
	MvnCell.cc \
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

