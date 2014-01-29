## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	testvect \
	bddsim \
	misc \
	io_cmds \
	calc_cvf \
	calc_svf \
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
	seal_tclsh


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CXXFLAGS = \
	-DSEAL_LIBDIR='"$(SEAL_LIBDIR)"' \
	-DSEAL_DATADIR='"$(SEAL_DATADIR)"' \
	-DSEAL_BINDIR='"$(SEAL_BINDIR)"'

AM_CPPFLAGS = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)


############################################################
## seal_tclsh 用の変数の定義
############################################################

seal_tclsh_SOURCES = \
	tclAppInit.cc \
	seal_init.cc \
	SealMgr.cc \
	SealCmd.cc

seal_tclsh_LDADD = \
	calc_svf/libseal_calc_svf.la \
	calc_cvf/libseal_calc_cvf.la \
	bddsim/libseal_bddsim.la \
	io_cmds/libseal_io_cmds.la \
	testvect/libseal_testvect.la \
	misc/libseal_misc.la \
	$(YM_LIBS_LIBYM_LEXP) \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBYM_UTILS) \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)

seal_tclsh_LDFLAGS =
