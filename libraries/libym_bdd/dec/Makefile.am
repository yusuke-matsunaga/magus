## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIR =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_bdd_dec.la


############################################################
## Variables for building libym_bdd_dec.la
############################################################

libym_bdd_dec_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(GMP_INCLUDES)

libym_bdd_dec_la_SOURCES = \
	Dg.cc \
	DgNode.cc \
	DgNode.h \
	DgMgr.cc \
	Bidec.cc \
	PrimDec.cc

libym_bdd_dec_la_LIBADD =

libym_bdd_dec_la_LDFLAGS =

