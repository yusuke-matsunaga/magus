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
	libesp.la \
	libesp_d.la \
	libesp_p.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include \
	-I$(top_srcdir)/src


############################################################
## Common Variables
############################################################

COMMON_SOURCES = \
	ansi.h \
	cols.c \
	copyright.h \
	cpu_time.c \
	dominate.c \
	espresso.h \
	gimpel.c \
	indep.c \
	matrix.c \
	mincov.c \
	mincov.h \
	mincov_int.h \
	part.c \
	port.h \
	prtime.c \
	rows.c \
	solution.c \
	sparse.c \
	sparse.h \
	sparse_int.h \
	utility.h


############################################################
## Variables for building mincov_test
############################################################

libesp_la_CFLAGS = \
	$(YM_CXXOPTFLAGS) \
	$(AM_CFLAGS)

libesp_la_SOURCES = \
	$(COMMON_SOURCES)

libesp_la_LIBADD =

libesp_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building libesp_la
############################################################

libesp_d_la_CFLAGS = \
	$(YM_CXXDBGFLAGS) \
	$(AM_CFLAGS)

libesp_d_la_SOURCES = \
	$(COMMON_SOURCES)

libesp_d_la_LIBADD =

libesp_d_la_LDFLAGS = \
	$(AM_LDFLAGS)


############################################################
## Variables for building libesp_la
############################################################

libesp_p_la_CFLAGS = \
	$(YM_CXXPRFFLAGS) \
	$(AM_CFLAGS)

libesp_p_la_SOURCES = \
	$(COMMON_SOURCES)

libesp_p_la_LIBADD =

libesp_p_la_LDFLAGS = \
	$(AM_LDFLAGS)
