## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

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
	verilog \
	. \
	tests


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST = \
	configure \
	configure.ac


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libym_networks.la


############################################################
## libym_network.la 用の変数の定義
############################################################

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
	conv_mvn_bdn/libym_conv_mvn_bdn.la \
	verilog/libym_vlw.la

libym_networks_la_LDFLAGS =

# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libym_networks_la_SOURCES = dummy.cc
