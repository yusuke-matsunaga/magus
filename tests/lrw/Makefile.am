## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS =

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	@POPT_INCLUDES@


noinst_PROGRAMS = \
	npngen \
	cannonical \
	ident \
	read_table \
	xform_comp_gen \
	npntest \
	npnnodemgr_test \
	genpat \
	normalize \
	lrw

npngen_SOURCES = \
	npngen.cc \
	NpnXform.h \
	NpnXform.cc

npngen_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

cannonical_SOURCES = \
	cannonical.cc \
	NpnXform.h \
	NpnXform.cc

normalize_SOURCES = \
	normalize.cc \
	NpnXform.h \
	NpnXform.cc

ident_SOURCES = \
	ident.cc \
	NpnXform.h \
	NpnXform.cc

read_table_SOURCES = \
	read_table.cc \
	abc_data.cc \
	RwtMgr.h \
	RwtMgr.cc \
	RwtNode.h

read_table_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

xform_comp_gen_SOURCES = \
	xform_comp_gen.cc

xform_comp_gen_LDADD =

npntest_SOURCES = \
	NpnXform.h \
	NpnXform.cc \
	npntest.cc

npntest_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

npnnodemgr_test_SOURCES = \
	NpnXform.h \
	NpnXform.cc \
	NpnNode.h \
	NpnNodeMgr.h \
	NpnNodeMgr.cc \
	npnnodemgr_test.cc

npnnodemgr_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

genpat_SOURCES = \
	NpnXform.h \
	NpnXform.cc \
	NpnHandle.h \
	NpnNode.h \
	NpnNodeMgr.h \
	NpnNodeMgr.cc \
	GenPat.h \
	GenPat.cc \
	GenPat2.h \
	GenPat2.cc \
	GpMgr.h \
	GpMgr.cc \
	GpNode.h \
	genpat.cc \
	npn4cannon.h \
	npn4ident.h \
	npn4norm.h \
	npn4perm.h \
	npn4rep.h

genpat_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

lrw_SOURCES = \
	AigTemplate.h \
	AigTemplate.cc \
	LrMgr.h \
	LrMgr.cc \
	LrMgr_init_table.cc \
	lrtable.h \
	lrw.cc

lrw_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@
