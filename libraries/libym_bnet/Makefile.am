## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_bnet \
	. \
	tests

INCLUDES = \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libym_bnet.la
libym_bnet_la_LIBADD = \
	$(top_builddir)/libym_blif/libym_blif.la \
	$(top_builddir)/libym_iscas89/libym_iscas89.la \
	$(top_builddir)/libym_lexp/libym_lexp.la \
	$(top_builddir)/libym_utils/libym_utils.la
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
