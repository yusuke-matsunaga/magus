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
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old_atpg/include


############################################################
## atpg_tclsh 用の変数の定義
############################################################

atpg_tclsh_SOURCES = \
	tclAppInit.cc \
	atpg_init.cc \
	AtpgCmd.h \
	AtpgCmd.cc \
	AtpgMsgHandler.h \
	AtpgMsgHandler.cc \
	NtwkIoCmd.h \
	NtwkIoCmd.cc \
	FaultCmd.h \
	FaultCmd.cc \
	PrintFaultCmd.h \
	PrintFaultCmd.cc \
	PrintPatCmd.h \
	PrintPatCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	RfsimCmd.h \
	RfsimCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc

atpg_tclsh_LDADD = \
	$(top_builddir)/old_atpg/src/main/libatpg_main.la \
	$(top_builddir)/old_atpg/src/dtpg_classic/libatpg_dtpg_cl.la \
	$(top_builddir)/old_atpg/src/dtpg_sat/libatpg_dtpg_sat.la \
	$(top_builddir)/old_atpg/src/fsim/libatpg_fsim.la \
	$(top_builddir)/old_atpg/src/fsim3/libatpg_fsim3.la \
	$(top_builddir)/old_atpg/src/fsimx/libatpg_fsimx.la \
	$(top_builddir)/old_atpg/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

atpg_tclsh_LDFLAGS =
