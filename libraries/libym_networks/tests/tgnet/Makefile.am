## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	lmtest

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/tgnet

lmtest_SOURCES = \
	lmtest.cc

lmtest_LDADD = \
	../../libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_iscas89/libym_iscas89.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
