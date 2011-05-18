## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/tgnet

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	lmtest

lmtest_SOURCES = \
	lmtest.cc

lmtest_LDADD = \
	../../libym_networks.la
