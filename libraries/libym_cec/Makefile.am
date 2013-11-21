## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS = \
	ym_cec \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cec.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_cec_la_SOURCES = \
	FraigHash.h \
	FraigHandle.h \
	FraigNode.h \
	FraigNode.cc \
	FraigMgrImpl.h \
	FraigMgrImpl.cc \
	FraigMgr.h \
	FraigMgr.cc \
	check_ceq.cc \
	CecHandle.h \
	CecHash.h \
	CecMgr.h \
	CecMgr.cc \
	CecNode.h \
	check_ceq2.cc \
	make_miter.cc

libym_cec_la_LIBADD =

libym_cec_la_LDFLAGS =
