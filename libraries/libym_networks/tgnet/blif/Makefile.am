## Process this file with automake to produce Makefile.in

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_tgblifreader.la

libym_tgblifreader_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/blif

libym_tgblifreader_la_SOURCES = \
	TgBlifHandler.h \
	TgBlifHandler.cc \
	TgBlifReader.cc

libym_tgblifreader_la_LIBADD =

libym_tgblifreader_la_LDFLAGS =
