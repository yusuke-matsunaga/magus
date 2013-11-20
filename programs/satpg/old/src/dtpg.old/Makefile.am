## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dtpg_old.la

libatpg_dtpg_old_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/old/include

libatpg_dtpg_old_la_SOURCES = \
	dtpg_nsdef.h \
	DtpgNode.h \
	DtpgNode.cc \
	DtpgFault.h \
	DtpgFFR.h \
	DtpgNetwork.h \
	DtpgNetwork.cc \
	DtpgSat.h \
	DtpgSat.cc \
	InputLiteral.h \
	InputLiteral.cc

libatpg_dtpg_old_la_LIBADD =

libatpg_dtpg_old_la_LDFLAGS =
