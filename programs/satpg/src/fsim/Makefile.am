## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	fsim2 \
	fsim3 \
	fop


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
	libatpg_fsim.la \
	libatpg_fsim_d.la \
	libatpg_fsim_p.la


############################################################
## libatpg_fsim.la 用の変数の定義
############################################################

libatpg_fsim_la_SOURCES =

nodist_EXTRA_libatpg_fsim_la_SOURCES = dummy.cc

libatpg_fsim_la_LIBADD = \
	fsim2/libatpg_fsim2.la \
	fsim3/libatpg_fsim3.la \
	fop/libatpg_fop.la

libatpg_fsim_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_fsim_d.la 用の変数の定義
############################################################

libatpg_fsim_d_la_SOURCES =

nodist_EXTRA_libatpg_fsim_d_la_SOURCES = dummy.cc

libatpg_fsim_d_la_LIBADD = \
	fsim2/libatpg_fsim2_d.la \
	fsim3/libatpg_fsim3_d.la \
	fop/libatpg_fop_d.la

libatpg_fsim_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## libatpg_fsim_p.la 用の変数の定義
############################################################

libatpg_fsim_p_la_SOURCES =

nodist_EXTRA_libatpg_fsim_p_la_SOURCES = dummy.cc

libatpg_fsim_p_la_LIBADD = \
	fsim2/libatpg_fsim2_p.la \
	fsim3/libatpg_fsim3_p.la \
	fop/libatpg_fop_p.la

libatpg_fsim_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
