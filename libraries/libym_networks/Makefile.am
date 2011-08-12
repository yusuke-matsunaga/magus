## Process this file with automake to produce Makefile.in

SUBDIRS = \
	ym_networks \
	blif \
	iscas89 \
	bdn \
	bnet \
	cmn \
	mvn \
	tgnet \
	conv_bnet_bdn \
	conv_mvn_bdn \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_networks.la

libym_networks_la_SOURCES =

libym_networks_la_LIBADD = \
	blif/libym_blif.la \
	iscas89/libym_iscas89.la \
	bdn/libym_bdn.la \
	bnet/libym_bnet.la \
	cmn/libym_cmn.la \
	mvn/libym_mvn.la \
	tgnet/libym_tgnet.la \
	conv_bnet_bdn/libym_conv_bnet_bdn.la \
	conv_mvn_bdn/libym_conv_mvn_bdn.la

libym_networks_la_LDFLAGS =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_networks_la_SOURCES = dummy.cc

