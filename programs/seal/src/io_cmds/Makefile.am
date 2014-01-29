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

noinst_LTLIBRARIES = libseal_io_cmds.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)


############################################################
## libseal_io_cmds.la 用の変数の定義
############################################################

libseal_io_cmds_la_SOURCES = \
	SealMsgHandler.h \
	SealMsgHandler.cc \
	NtwkIoCmd.h \
	NtwkIoCmd.cc

libseal_io_cmds_la_LIBADD =

libseal_io_cmds_la_LDFLAGS = \
	$(YM_LIBS_LIBYM_TGNET) \
	$(YM_LIBS_LIBYM_BLIF) \
	$(YM_LIBS_LIBYM_ISCAS89)
