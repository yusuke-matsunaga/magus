## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 558 2007-04-09 05:54:44Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(TK_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_misc_tcl.la

libmagus_misc_tcl_la_SOURCES = \
	TimeCmd.h \
	TimeCmd.cc \
	RandCmd.h \
	RandCmd.cc \
	misc_init.cc

libmagus_misc_tcl_la_LIBADD = \
	$(YM_LIBS_LIBYM_TCLPP)
