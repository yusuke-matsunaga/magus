## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 958 2007-08-28 05:38:02Z matsunaga $

SUBDIRS =

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
	iscas89_nsdef.h \
	iscas89_grammer.yy \
	Iscas89Scanner.h \
	Iscas89Scanner.cc \
	IdHash.h \
	IdHash.cc \
	Iscas89Parser.h \
	Iscas89Parser.cc \
	Iscas89ParserImpl.h \
	Iscas89ParserImpl.cc \
	Iscas89Handler.h \
	Iscas89Handler.cc \
	Iscas89Network.h \
	Iscas89Network.cc \
	Iscas89NetworkHandler.h \
	Iscas89NetworkHandler.cc \
	Iscas89NetworkReader.h \
	Iscas89NetworkReader.cc

libym_iscas89_la_LIBADD =

libym_iscas89_la_LDFLAGS =
