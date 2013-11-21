## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = \
	sat_engine \
	bt \
	dop \
	uop

MAINTAINERCLEANFILES = Makefile.in


noinst_LTLIBRARIES = libatpg_dtpg.la

libatpg_dtpg_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src \
	$(GMP_INCLUDES)

libatpg_dtpg_la_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc \
	SatEngine.h

libatpg_dtpg_la_LDFLAGS =

libatpg_dtpg_la_LIBADD = \
	sat_engine/libatpg_sat_engine.la \
	bt/libatpg_bt.la \
	dop/libatpg_dop.la \
	uop/libatpg_uop.la
