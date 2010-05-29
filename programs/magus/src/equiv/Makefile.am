## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/programs/magus/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv.la

libmagus_equiv_la_LIBADD = \
	$(top_builddir)/libraries/libym_aig/libym_aig.la \
	$(top_builddir)/libraries/libym_sat/libym_sat.la \
	$(top_builddir)/libraries/libym_bnet/libym_bnet.la

libmagus_equiv_la_LDFLAGS =

libmagus_equiv_la_SOURCES = \
	equiv_nsdef.h \
	check_equiv.cc

