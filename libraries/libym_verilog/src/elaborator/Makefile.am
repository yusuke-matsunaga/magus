## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS = \
	include \
	main \
	elb_mgr \
	ei \
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

noinst_LTLIBRARIES = \
	libvlelab.la \
	libvlelab_d.la \
	libvlelab_p.la


############################################################
## libvlelab.la 用の変数の定義
############################################################

libvlelab_la_SOURCES =

libvlelab_la_LIBADD = \
	main/libvlelab_main.la \
	elb_mgr/libvlelab_mgr.la \
	ei/libvlelab_ei.la

libvlelab_la_LDFLAGS =


############################################################
## libvlelab_d.la 用の変数の定義
############################################################

libvlelab_d_la_SOURCES =

libvlelab_d_la_LIBADD = \
	main/libvlelab_main_d.la \
	elb_mgr/libvlelab_mgr_d.la \
	ei/libvlelab_ei_d.la

libvlelab_d_la_LDFLAGS =


############################################################
## libvlelab_p.la 用の変数の定義
############################################################

libvlelab_p_la_SOURCES =

libvlelab_p_la_LIBADD = \
	main/libvlelab_main_p.la \
	elb_mgr/libvlelab_mgr_p.la \
	ei/libvlelab_ei_p.la

libvlelab_p_la_LDFLAGS =
