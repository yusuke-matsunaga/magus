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

noinst_LTLIBRARIES = \
	libmatpg_tcl.la

bin_PROGRAMS = \
	matpg_tclsh


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old_matpg/include \
	-I$(top_srcdir)/old_matpg/src/libmatpg \
	-DMATPG_LIBDIR='"$(MATPG_LIBDIR)"' \
	-DMATPG_DATADIR='"$(MATPG_DATADIR)"' \
	-DMATPG_BINDIR='"$(MATPG_BINDIR)"'


############################################################
## libmatpg_tcl.la 用の変数の定義
############################################################

libmatpg_tcl_la_SOURCES = \
	MatpgMsgHandler.h \
	MatpgMsgHandler.cc \
	ReadBlifCmd.h \
	ReadBlifCmd.cc \
	ReadIscas89Cmd.h \
	ReadIscas89Cmd.cc \
	SetFaultCmd.h \
	SetFaultCmd.cc \
	PrintFaultCmd.h \
	PrintFaultCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	PrintPatCmd.h \
	PrintPatCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	RfsimCmd.h \
	RfsimCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc \
	matpg_init.cc

libmatpg_tcl_la_LIBADD =

libmatpg_tcl_la_LDFLAGS =


############################################################
## matpg_tclsh 用の変数の定義
############################################################

matpg_tclsh_SOURCES = \
	tclAppInit.cc

matpg_tclsh_LDADD = \
	libmatpg_tcl.la \
	$(top_builddir)/old_matpg/src/libmatpg/libmatpg.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LADD)

matpg_tclsh_LDFLAGS =
