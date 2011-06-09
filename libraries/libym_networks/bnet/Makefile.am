## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	$(YM_EXTRA_SUBDIRS) \
	.

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bnet.la

libym_bnet_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_bnet_la_SOURCES = \
	StrBNodeMap.h \
	StrBNodeMap.cc \
	BNodeMgr.h \
	BNodeMgr.cc \
	BNetwork.cc \
	eliminate.cc \
	BNode.cc \
	BNetManip.cc \
	BNetDecomp.cc \
	BNetGateDecomp.cc \
	BNetSopDecomp.cc \
	BNetBlifWriter.cc \
	BNetEquWriter.cc \
	BNetVerilogWriter.cc

libym_bnet_la_LIBADD = \
	$(YM_EXTRA_LIBADD)

libym_bnet_la_LDFLAGS =
