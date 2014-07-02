## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


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
	atpg_tclsh

noinst_PROGRAMS = \
	atpg_tclsh_d \
	atpg_tclsh_p


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CXXFLAGS = \
	-DATPG_LIBDIR='"$(ATPG_LIBDIR)"' \
	-DATPG_DATADIR='"$(ATPG_DATADIR)"' \
	-DATPG_BINDIR='"$(ATPG_BINDIR)"'

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_utils/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_logic/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_cell/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_networks/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_tclpp/include \
	-I$(YMTOOLS_BUILDDIR)/libraries/libym_utils/include \
	-I$(top_srcdir)/include


############################################################
## 共通の変数定義
############################################################

COMMON_SOURCES = \
	tclAppInit.cc \
	atpg_init.cc \
	AtpgCmd.h \
	AtpgCmd.cc \
	AtpgMsgHandler.h \
	AtpgMsgHandler.cc \
	NtwkIoCmd.h \
	NtwkIoCmd.cc \
	PrintFaultCmd.h \
	PrintFaultCmd.cc \
	PrintPatCmd.h \
	PrintPatCmd.cc \
	PrintPatStatsCmd.h \
	PrintPatStatsCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc \
	MinPatCmd.h \
	MinPatCmd.cc


############################################################
## atpg_tclsh 用の変数の定義
############################################################

atpg_tclsh_CXXFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CXXFLAGS)

atpg_tclsh_SOURCES = \
	$(COMMON_SOURCES)

atpg_tclsh_LDADD = \
	$(top_builddir)/src/main/libatpg_main.la \
	$(top_builddir)/src/dtpg/libatpg_dtpg.la \
	$(top_builddir)/src/graphsat/libatpg_graphsat.la \
	$(top_builddir)/src/bt/libatpg_bt.la \
	$(top_builddir)/src/dop/libatpg_dop.la \
	$(top_builddir)/src/uop/libatpg_uop.la \
	$(top_builddir)/src/rtpg/libatpg_rtpg.la \
	$(top_builddir)/src/minpat/libatpg_minpat.la \
	$(top_builddir)/src/fsim/libatpg_fsim.la \
	$(top_builddir)/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/src/libym_tclpp.la \
	$(TCLRL_LIBS) \
	$(TCL_LDADD)

atpg_tclsh_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## atpg_tclsh_d 用の変数の定義
############################################################

atpg_tclsh_d_CXXFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CXXFLAGS)

atpg_tclsh_d_SOURCES = \
	$(COMMON_SOURCES)

atpg_tclsh_d_LDADD = \
	$(top_builddir)/src/main/libatpg_main_d.la \
	$(top_builddir)/src/dtpg/libatpg_dtpg_d.la \
	$(top_builddir)/src/graphsat/libatpg_graphsat_d.la \
	$(top_builddir)/src/bt/libatpg_bt_d.la \
	$(top_builddir)/src/dop/libatpg_dop_d.la \
	$(top_builddir)/src/uop/libatpg_uop_d.la \
	$(top_builddir)/src/rtpg/libatpg_rtpg_d.la \
	$(top_builddir)/src/minpat/libatpg_minpat_d.la \
	$(top_builddir)/src/fsim/libatpg_fsim_d.la \
	$(top_builddir)/src/base/libatpg_base_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_d.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/src/libym_tclpp_d.la \
	$(TCLRL_LIBS) \
	$(TCL_LDADD)

atpg_tclsh_d_LDFLAGS = \
	-static \
	$(AM_LDFLAGS)


############################################################
## atpg_tclsh_p 用の変数の定義
############################################################

atpg_tclsh_p_CXXFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CXXFLAGS)

atpg_tclsh_p_SOURCES = \
	$(COMMON_SOURCES)

atpg_tclsh_p_LDADD = \
	$(top_builddir)/src/main/libatpg_main_p.la \
	$(top_builddir)/src/dtpg/libatpg_dtpg_p.la \
	$(top_builddir)/src/graphsat/libatpg_graphsat_p.la \
	$(top_builddir)/src/bt/libatpg_bt_p.la \
	$(top_builddir)/src/dop/libatpg_dop_p.la \
	$(top_builddir)/src/uop/libatpg_uop_p.la \
	$(top_builddir)/src/rtpg/libatpg_rtpg_p.la \
	$(top_builddir)/src/minpat/libatpg_minpat_p.la \
	$(top_builddir)/src/fsim/libatpg_fsim_p.la \
	$(top_builddir)/src/base/libatpg_base_p.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/src/libym_networks_p.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/src/libym_logic_p.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/src/libym_utils_p.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/src/libym_tclpp_p.la \
	$(TCLRL_LIBS) \
	$(TCL_LDADD)

atpg_tclsh_p_LDFLAGS = \
	-static \
	$(AM_LDFLAGS)
