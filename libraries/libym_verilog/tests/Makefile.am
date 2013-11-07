## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = . \
	vltest \
	$(YM_EXTRA_TESTS_SUBDIRS)

YFLAGS=-d -v

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES = \
	bvcalc.cc bvcalc.h bvcalc.output bvlex.cc

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = yacc.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS = \
	bvcalc

noinst_PROGRAMS = \
	testgen \
	scalarval_test \
	udpval_test \
	valuetype_test

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


#############################################################
# bvcalc
#############################################################

bvcalc_SOURCES = \
	bvcalc.yy \
	bvlex.ll

bvcalc_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


#############################################################
# testgen
#############################################################

testgen_SOURCES = \
	testgen.cc

testgen_LDADD =


#############################################################
# scalarval_test
#############################################################

scalarval_test_SOURCES = \
	scalarval_test.cc

scalarval_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


#############################################################
# udpval_test
#############################################################

udpval_test_SOURCES = \
	udpval_test.cc

udpval_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


#############################################################
# valuetype_test
#############################################################

valuetype_test_SOURCES = \
	valuetype_test.cc

valuetype_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
