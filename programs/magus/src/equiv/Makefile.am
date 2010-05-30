## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(YMLIBS_BUILDDIR) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv.la

libmagus_equiv_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_aig/libym_aig.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_sat/libym_sat.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

libmagus_equiv_la_LDFLAGS =

libmagus_equiv_la_SOURCES = \
	equiv_nsdef.h \
	check_equiv.cc

