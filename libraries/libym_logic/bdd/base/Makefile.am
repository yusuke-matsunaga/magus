## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_bdd_base.la


############################################################
## Variables for building libym_bdd_base.la
############################################################

libym_bdd_base_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/bdd \
	$(GMP_INCLUDES)

libym_bdd_base_la_SOURCES = \
	Bdd.cc \
	BddEdge.h \
	BddMgr.cc \
	BddMgrImpl.h \
	BddMgrImpl.cc \
	BddLitSet.cc \
	BddVarSet.cc \
	IdMgr.h \
	IdMgr.cc \
	Printer.h \
	Printer.cc \
	Dumper.h \
	Dumper.cc \
	Restorer.h \
	Restorer.cc

libym_bdd_base_la_LIBADD =

libym_bdd_base_la_LDFLAGS =

