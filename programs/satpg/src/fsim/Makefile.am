## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	fsim2 \
	fsim3 \
	fsimx \
	fsimx2


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

noinst_LTLIBRARIES = libatpg_fsim.la


############################################################
## libatpg_fsim.la 用の変数の定義
############################################################

libatpg_fsim_la_SOURCES =

libatpg_fsim_la_LIBADD = \
	fsim2/libatpg_fsim2.la \
	fsim3/libatpg_fsim3.la \
	fsimx/libatpg_fsimx.la \
	fsimx2/libatpg_fsimx2.la

libatpg_fsim_la_LDFLAGS =
