## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_vl_common.la

libym_vl_common_la_SOURCES = \
	BitVector.cc \
	udpval.cc

libym_vl_common_la_LIBADD = \
	$(top_builddir)/libraries/libym_utils/libym_utils.la
libym_vl_common_la_LDFLAGS =
