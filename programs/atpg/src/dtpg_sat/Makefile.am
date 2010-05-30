## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = 

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMLIBS_BUILDDIR) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dtpg_sat.la

libatpg_dtpg_sat_la_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc \
	tgnet2aig.cc

libatpg_dtpg_sat_la_LDFLAGS =

libatpg_dtpg_sat_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_aig/libym_aig.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_sat/libym_sat.la
