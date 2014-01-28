## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libseal_misc.la

AM_CPPFLAGS = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)

libseal_misc_la_SOURCES = \
	TimeCmd.h \
	TimeCmd.cc \
	misc_init.cc
libseal_misc_la_LIBADD = \
	$(YM_LIBS_LIBYM_TCLPP)
