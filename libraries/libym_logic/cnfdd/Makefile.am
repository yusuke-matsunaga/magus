## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic_cnfdd.la


############################################################
## Variables for building libym_logic_cnfdd.la
############################################################

libym_logic_cnfdd_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES)

libym_logic_cnfdd_la_SOURCES = \
	CompTbl.h \
	CompTbl.cc \
	CNFddEdge.h \
	CNFddNode.h \
	CNFddVar.h \
	CNFdd.cc \
	CNFddMgr.cc \
	CNFddMgrImpl.h \
	CNFddMgrImpl.cc \
	CNFddOp.h \
	ConOp.h \
	ConOp.cc \
	DiffOp.h \
	DiffOp.cc \
	AddLiteralOp.h \
	AddLiteralOp.cc \
	Cof0Op.h \
	Cof0Op.cc \
	CofPOp.h \
	CofPOp.cc \
	CofNOp.h \
	CofNOp.cc \
	count.cc \
	support.cc \
	IdMgr.h \
	IdMgr.cc \
	Printer.h \
	Printer.cc \
	Dumper.h \
	Dumper.cc \
	Restorer.h \
	Restorer.cc

libym_logic_cnfdd_la_LIBADD =

libym_logic_cnfdd_la_LDFLAGS =

