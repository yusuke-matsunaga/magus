## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/tests/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS = .

if WITH_QT
SUBDIRS += dotlib_ptview
endif

AM_CPPFLAGS = \
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

YM_CELL = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

bin_PROGRAMS = \
	mislib_parser_test \
	mislib_reader_test \
	dotlib_parser_test \
	dotlib_reader_test \
	dump_restore_test \
	libdump_test \
	cell_mgr_test \
	cell_dump_test

mislib_parser_test_SOURCES = \
	mislib_parser_test.cc

mislib_parser_test_LDFLAGS =

mislib_parser_test_LDADD = \
	$(YM_CELL)


mislib_reader_test_SOURCES = \
	mislib_reader_test.cc

mislib_reader_test_LDFLAGS =

mislib_reader_test_LDADD = \
	$(YM_CELL)


dotlib_reader_test_SOURCES = \
	dotlib_reader_test.cc

dotlib_reader_test_LDFLAGS =

dotlib_reader_test_LDADD = \
	$(YM_CELL)


dotlib_parser_test_SOURCES = \
	dotlib_parser_test.cc

dotlib_parser_test_LDFLAGS =

dotlib_parser_test_LDADD = \
	$(YM_CELL)


dump_restore_test_SOURCES = \
	dump_restore_test.cc

dump_restore_test_LDFLAGS =

dump_restore_test_LDADD = \
	$(YM_CELL)


cell_mgr_test_SOURCES = \
	cell_mgr_test.cc

cell_mgr_test_LDADD = \
	$(YM_CELL)


cell_dump_test_SOURCES = \
	cell_dump_test.cc

cell_dump_test_LDADD = \
	$(YM_CELL)


libdump_test_SOURCES = \
	libdump_test.cc

libdump_test_LDADD = \
	$(YM_CELL)
