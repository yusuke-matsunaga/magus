## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1508 2008-06-30 04:55:42Z matsunaga $

SUBDIRS = \
	utils \
	.

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_logic_npn.la

libym_logic_npn_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_logic_npn_la_SOURCES = \
	NpnRawSig.h \
	NpnRawSig.cc \
	NpnConf.h \
	NpnConf.cc \
	NpnMgr.cc \
	NpnMap.cc

libym_logic_npn_la_LIBADD =

libym_logic_npn_la_LDFLAGS =
