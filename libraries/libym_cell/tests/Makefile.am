## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/tests/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = unit.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS = \
	mislib_reader_test \
	dotlib_reader_test \
	dump_restore_test

mislib_reader_test_SOURCES = \
	mislib_reader_test.cc

mislib_reader_test_LDFLAGS =

mislib_reader_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la

dotlib_reader_test_SOURCES = \
	dotlib_reader_test.cc

dotlib_reader_test_LDFLAGS =

dotlib_reader_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la

dump_restore_test_SOURCES = \
	dump_restore_test.cc

dump_restore_test_LDFLAGS =

dump_restore_test_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la
