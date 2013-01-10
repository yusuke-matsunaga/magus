## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dtpg.la

libatpg_dtpg_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libatpg_dtpg_la_SOURCES = \
	dtpg_nsdef.h \
	DtpgNode.h \
	DtpgFault.h \
	DtpgFFR.h \
	DtpgNetwork.h \
	DtpgNetwork.cc \
	prime_gen.cc \
	DtpgSat.h \
	DtpgSat.cc

libatpg_dtpg_la_LIBADD =

libatpg_dtpg_la_LDFLAGS =
