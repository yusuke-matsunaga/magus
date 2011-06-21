## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	blif \
	.

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdn.la

libym_bdn_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_bdn_la_SOURCES = \
	BdnBlifWriter.cc \
	BdnDumper.cc \
	BdnMgr.cc \
	BdnMgrImpl.h \
	BdnMgrImpl.cc \
	BdnNode1.h \
	BdnNode2.h \
	BdnNode.cc \
	BdnVerilogWriter.cc

libym_bdn_la_LIBADD = \
	blif/libym_bdn_blifreader.la

libym_bdn_la_LDFLAGS =


