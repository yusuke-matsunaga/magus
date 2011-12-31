## Process this file with automake to produce Makefile.in

SUBDIR =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_bdd_bmm.la


############################################################
## Variables for building libym_bdd_bmm.la
############################################################

libym_bdd_bmm_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/bdd/include \
	$(GMP_INCLUDES)

libym_bdd_bmm_la_SOURCES = \
	BddMgrModern.h \
	BddMgrModern.cc \
	BmmVar.h \
	BmmVar.cc \
	BmmCompTbl.h \
	BmmCompTbl.cc \
	bmm_ite.cc \
	bmm_sup.cc \
	bmm_count.cc \
	bmm_isop.cc \
	bmm_onepath.cc \
	bmm_vs.cc \
	bmm_ls.cc

libym_bdd_bmm_la_LIBADD =

libym_bdd_bmm_la_LDFLAGS = \
	$(GMP_LIBS)
