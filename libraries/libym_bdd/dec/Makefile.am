## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIR =

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libym_bdd \
	$(GMP_INCLUDES)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdd_dec.la

libym_bdd_dec_la_LIBADD =

libym_bdd_dec_la_LDFLAGS =

libym_bdd_dec_la_SOURCES = \
	Dg.cc \
	DgNode.cc \
	DgNode.h \
	DgMgr.cc \
	Bidec.cc \
	PrimDec.cc

