## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv.la

libmagus_equiv_la_LIBADD = \
	$(YM_LIBS_LIBYM_AIG) \
	$(YM_LIBS_LIBYM_SAT) \
	$(YM_LIBS_LIBYM_BNET)

libmagus_equiv_la_LDFLAGS =

libmagus_equiv_la_SOURCES = \
	equiv_nsdef.h \
	check_equiv.cc

