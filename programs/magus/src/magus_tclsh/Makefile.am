## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = include logbase equiv lutmap seal simplify misc test .

AM_CXXFLAGS = \
	-DMAGUS_LIBDIR='"$(MAGUS_LIBDIR)"' \
	-DMAGUS_DATADIR='"$(MAGUS_DATADIR)"' \
	-DMAGUS_BINDIR='"$(MAGUS_BINDIR)"'

INCLUDES = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/magus_tclsh/include

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
	../equiv/libmagus_equiv.la \
	../lutmap/libmagus_lutmap.la \
	../seal/libmagus_seal.la \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)




