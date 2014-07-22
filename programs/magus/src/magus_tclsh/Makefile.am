## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

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


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

bin_PROGRAMS = \
	magus_tclsh


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_tclpp/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/magus_tclsh/include \
	-DMAGUS_LIBDIR='"$(MAGUS_LIBDIR)"' \
	-DMAGUS_DATADIR='"$(MAGUS_DATADIR)"' \
	-DMAGUS_BINDIR='"$(MAGUS_BINDIR)"'


############################################################
## magus_tclsh 用の変数の定義
############################################################

magus_tclsh_SOURCES = \
	magus_init.cc \
	tclAppInit.cc

magus_tclsh_LDADD = \
	core/libmagus_core_tcl.la \
	bnet/libmagus_bnet_tcl.la \
	mvn/libmagus_mvn_tcl.la \
	equiv/libmagus_equiv_tcl.la \
	lutmap/libmagus_lutmap_tcl.la \
	techmap/libmagus_techmap_tcl.la \
	simplify/libmagus_simplify_tcl.la \
	seal/libmagus_seal_tcl.la \
	misc/libmagus_misc_tcl.la \
	test/libmagus_test_tcl.la \
	../cequiv/libcequiv.la \
	../seal/libseal.la \
	../techmap/libtechmap.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/src/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/src/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/src/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

magus_tclsh_LDFLAGS =
