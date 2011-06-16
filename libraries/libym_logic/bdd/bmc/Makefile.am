## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIR =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_bdd_bmc.la


############################################################
## Variables for building libym_bdd_bmc.la
############################################################

libym_bdd_bmc_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/bdd \
	$(GMP_INCLUDES)

libym_bdd_bmc_la_SOURCES = \
	BddMgrClassic.h \
	BddMgrClassic.cc \
	BmcNode.h \
	BmcCompTbl.h \
	BmcCompTbl.cc \
	bmc_and.cc \
	bmc_ite.cc \
	bmc_sup.cc \
	bmc_count.cc \
	bmc_isop.cc \
	bmc_onepath.cc \
	bmc_symmetry.cc \
	bmc_vs.cc \
	bmc_ls.cc

libym_bdd_bmc_la_LIBADD =

libym_bdd_bmc_la_LDFLAGS = \
	$(GMP_LIBS)
