## Process this file with automake to produce Makefile.in

SUBDIRS = \
	blif \
	iscas89 \
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
	blif/libym_bnetblifreader.la \
	iscas89/libym_bnetiscas89reader.la

libym_bnet_la_LDFLAGS =
