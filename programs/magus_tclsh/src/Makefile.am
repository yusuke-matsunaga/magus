## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	core \
	bnet \
	mvn \
	equiv \
	lutmap \
	techmap \
	seal \
	simplify \
	misc \
	test \
	.

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-DMAGUS_LIBDIR='"$(MAGUS_LIBDIR)"' \
	-DMAGUS_DATADIR='"$(MAGUS_DATADIR)"' \
	-DMAGUS_BINDIR='"$(MAGUS_BINDIR)"'

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	magus_tclsh

magus_tclsh_SOURCES = \
	magus_init.cc \
	tclAppInit.cc

magus_tclsh_LDADD = \
	equiv/libmagus_equiv_tcl.la \
	lutmap/libmagus_lutmap_tcl.la \
	techmap/libmagus_techmap_tcl.la \
	seal/libmagus_seal_tcl.la \
	mvn/libmagus_mvn_tcl.la \
	bnet/libmagus_bnet_tcl.la \
	core/libmagus_core_tcl.la \
	misc/libmagus_misc_tcl.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_seal/libym_seal.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_techmap/libym_techmap.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cec/libym_cec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_iscas89/libym_iscas89.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_dotlib/libym_dotlib.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_mislib/libym_mislib.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_npn/libym_npn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sat/libym_sat.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)


