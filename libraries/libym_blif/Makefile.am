## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_blif \
	. \
	tests

INCLUDES = \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libym_blif.la

libym_blif_la_LIBADD = \
	$(top_builddir)/libym_utils/libym_utils.la

libym_blif_la_LDFLAGS = 

libym_blif_la_SOURCES = \
	BlifDic.h \
	BlifDic.cc \
	BlifScanner.h \
	BlifScanner.cc \
	IdHash.h \
	IdHash.cc \
	BlifParserImpl.h \
	BlifParserImpl.cc \
	BlifHandler.cc \
	BlifParser.cc \
	BlifNetworkReader.cc
