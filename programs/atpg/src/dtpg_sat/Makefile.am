## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = 

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dtpg_sat.la

libatpg_dtpg_sat_la_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc \
	tgnet2aig.cc

libatpg_dtpg_sat_la_LDFLAGS =

libatpg_dtpg_sat_la_LIBADD = \
	$(YM_LIBS_LIBYM_LEXP) \
	$(YM_LIBS_LIBYM_AIG) \
	$(YM_LIBS_LIBYM_SAT)
