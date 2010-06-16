## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_bnet \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bnet.la

libym_bnet_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_iscas89/libym_iscas89.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_bnet_la_LDFLAGS = 

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
	BNetIscas89Reader.cc \
	BNetBlifWriter.cc \
	BNetEquWriter.cc \
	BNetVerilogWriter.cc \
	BlifBNetConv.cc
