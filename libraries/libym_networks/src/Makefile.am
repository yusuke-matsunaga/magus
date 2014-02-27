## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	blif \
	iscas89 \
	bdn \
	bnet \
	cmn \
	mvn \
	tgnet \
	conv_mvn_bdn \
	verilog


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

noinst_LTLIBRARIES = \
	libym_networks.la \
	libym_networks_d.la \
	libym_networks_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../libym_utils/include \
	-I$(top_srcdir)/../libym_logic/include \
	-I$(top_builddir)/../libym_utils/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = BNetBdnConv.cc


############################################################
## libym_network.la 用の変数の定義
############################################################

libym_networks_la_CXXFLAGS = $(YM_CXXOPTFLAGS)

libym_networks_la_SOURCES = $(COMMON_SOURCES)

libym_networks_la_LIBADD = \
	blif/libblif.la \
	iscas89/libiscas89.la \
	bdn/libbdn.la \
	bnet/libbnet.la \
	cmn/libcmn.la \
	mvn/libmvn.la \
	tgnet/libtgnet.la \
	conv_mvn_bdn/libconv_mvn_bdn.la \
	verilog/libvlw.la

libym_networks_la_LDFLAGS = \
	$(top_builddir)/../libym_verilog/src/libym_verilog.la \
	$(top_builddir)/../libym_logic/src/libym_logic.la \
	$(top_builddir)/../libym_cell/src/libym_cell.la \
	$(top_builddir)/../libym_utils/src/libym_utils.la


############################################################
## libym_network_d.la 用の変数の定義
############################################################

libym_networks_d_la_CXXFLAGS = $(YM_CXXDBGFLAGS)

libym_networks_d_la_SOURCES = $(COMMON_SOURCES)

libym_networks_d_la_LIBADD = \
	blif/libblif_d.la \
	iscas89/libiscas89_d.la \
	bdn/libbdn_d.la \
	bnet/libbnet_d.la \
	cmn/libcmn_d.la \
	mvn/libmvn_d.la \
	tgnet/libtgnet_d.la \
	conv_mvn_bdn/libconv_mvn_bdn_d.la \
	verilog/libvlw_d.la

libym_networks_d_la_LDFLAGS = \
	$(top_builddir)/../libym_verilog/src/libym_verilog_d.la \
	$(top_builddir)/../libym_logic/src/libym_logic_d.la \
	$(top_builddir)/../libym_cell/src/libym_cell_d.la \
	$(top_builddir)/../libym_utils/src/libym_utils_d.la


############################################################
## libym_network_p.la 用の変数の定義
############################################################

libym_networks_p_la_CXXFLAGS = $(YM_CXXPRFFLAGS)

libym_networks_p_la_SOURCES = $(COMMON_SOURCES)

libym_networks_p_la_LIBADD = \
	blif/libblif_p.la \
	iscas89/libiscas89_p.la \
	bdn/libbdn_p.la \
	bnet/libbnet_p.la \
	cmn/libcmn_p.la \
	mvn/libmvn_p.la \
	tgnet/libtgnet_p.la \
	conv_mvn_bdn/libconv_mvn_bdn_p.la \
	verilog/libvlw_p.la

libym_networks_p_la_LDFLAGS = \
	$(top_builddir)/../libym_verilog/src/libym_verilog_p.la \
	$(top_builddir)/../libym_logic/src/libym_logic_p.la \
	$(top_builddir)/../libym_cell/src/libym_cell_p.la \
	$(top_builddir)/../libym_utils/src/libym_utils_p.la
