## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS = \
	ym_tclpp \
	. \
	examples

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_tclpp.la


############################################################
## Variables for building libym_tclpp.la
############################################################

libym_tclpp_la_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_tclpp_la_LIBADD =

libym_tclpp_la_LDFLAGS = \
	$(TCL_LDADD)

libym_tclpp_la_SOURCES = \
	StrBuf.h \
	TclPoptGroup.h \
	TclObj.cc \
	TclBase.cc \
	TclCmd.cc \
	TclPopt.cc \
	TclVarTrace.cc \
	TclHandler.cc
