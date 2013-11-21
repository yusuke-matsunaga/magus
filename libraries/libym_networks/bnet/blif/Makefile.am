## Process this file with automake to produce Makefile.in

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bnetblifreader.la

libym_bnetblifreader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/blif

libym_bnetblifreader_la_SOURCES = \
	BNetBlifHandler.h \
	BNetBlifHandler.cc \
	BNetBlifReader.cc

libym_bnetblifreader_la_LIBADD =

libym_bnetblifreader_la_LDFLAGS =
