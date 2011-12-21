## Process this file with automake to produce Makefile.in

SUBDIRS =

CLEANFILES =

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
	-I$(top_srcdir)/bdd/include \
	$(GMP_INCLUDES)

libym_bdd_base_la_SOURCES = \
	Bdd.cc \
	BddMgr.cc \
	BddMgrImpl.cc \
	BddLitSet.cc \
	BddVarSet.cc \
	BddVector.cc \
	BddList.cc \
	CompTbl.h \
	CompTbl.cc \
	CompTbl1.h \
	CompTbl1.cc \
	CompTbl2.h \
	CompTbl2.cc \
	CompTbl2e.h \
	CompTbl2e.cc \
	CompTbl3.h \
	CompTbl3.cc \
	BddOp.h \
	AndOp.h \
	AndOp.cc \
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

