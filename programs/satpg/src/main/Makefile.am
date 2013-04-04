## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_main.la

libatpg_main_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

libatpg_main_la_SOURCES = \
	AtpgMgr.cc \
	RtpgOld.h \
	RtpgOld.cc \
	RtpgImpl.h \
	RtpgImpl.cc \
	Fop2Rtpg.h \
	Fop2Rtpg.cc

libatpg_main_la_LDFLAGS =

libatpg_main_la_LIBADD =
