## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = -I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libym_bdd \
	$(GMP_INCLUDES)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdd_base.la

libym_bdd_base_la_LIBADD =

libym_bdd_base_la_LDFLAGS =

libym_bdd_base_la_SOURCES = \
	BddMgr.h \
	Dumper.h \
	Bdd.cc \
	BddMgr.cc \
	BddMgrRef.cc \
	BmcFactory.cc \
	BmmFactory.cc \
	BddLitSet.cc \
	BddVarSet.cc \
	Dumper.cc

