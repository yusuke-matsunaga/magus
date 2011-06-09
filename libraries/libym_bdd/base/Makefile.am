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
	-I$(top_srcdir) \
	$(GMP_INCLUDES)

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

libym_bdd_base_la_LIBADD =

libym_bdd_base_la_LDFLAGS =

