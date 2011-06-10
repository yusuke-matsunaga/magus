## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dtpg_sat.la

libatpg_dtpg_sat_la_CPPFLAGS = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libatpg_dtpg_sat_la_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc

libatpg_dtpg_sat_la_LIBADD =

libatpg_dtpg_sat_la_LDFLAGS =
