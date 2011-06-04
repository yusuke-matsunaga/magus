## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_networks \
	bdn \
	bnet \
	cmn \
	mvn \
	tgnet \
	conv_blif_bnet \
	conv_bnet_bdn \
	conv_mvn_bdn \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_networks.la

libym_networks_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_networks_la_LDFLAGS = \
	bdn/libym_bdn.la \
	bnet/libym_bnet.la \
	mvn/libym_mvn.la \
	tgnet/libym_tgnet.la \
	conv_blif_bnet/libym_conv_blif_bnet.la \
	conv_bnet_bdn/libym_conv_bnet_bdn.la \
	conv_mvn_bdn/libym_conv_mvn_bdn.la

libym_networks_la_SOURCES =

