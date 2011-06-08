## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 958 2007-08-28 05:38:02Z matsunaga $

SUBDIRS = \
	ym_iscas89 \
	. \
	tests

YFLAGS =-d -v

CLEANFILES = \
	iscas89_grammer.cc iscas89_grammer.h y.output iscas89_grammer.output

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_iscas89.la


############################################################
## Variables for building libym_iscas89.la
############################################################

libym_iscas89_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_iscas89_la_SOURCES = \
	iscas89_grammer.yy \
	Iscas89Scanner.h \
	Iscas89Scanner.cc \
	IdHash.h \
	IdHash.cc \
	Iscas89ParserImpl.h \
	Iscas89ParserImpl.cc \
	Iscas89Parser.cc \
	Iscas89Handler.cc

libym_iscas89_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_iscas89_la_LDFLAGS =
