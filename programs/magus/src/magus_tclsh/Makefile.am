## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	logbase \
	equiv \
	lutmap \
	seal \
	simplify \
	misc \
	test \
	.

AM_CXXFLAGS = \
	-DMAGUS_LIBDIR='"$(MAGUS_LIBDIR)"' \
	-DMAGUS_DATADIR='"$(MAGUS_DATADIR)"' \
	-DMAGUS_BINDIR='"$(MAGUS_BINDIR)"'

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	magus_tclsh

magus_tclsh_SOURCES = \
	magus_init.cc \
	tclAppInit.cc

magus_tclsh_LDADD = \
	logbase/libmagus_logbase_tcl.la \
	equiv/libmagus_equiv_tcl.la \
	lutmap/libmagus_lutmap_tcl.la \
	seal/libmagus_seal_tcl.la \
	misc/libmagus_misc_tcl.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)




