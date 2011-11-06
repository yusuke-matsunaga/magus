## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS = \
	base \
	.

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic_zdd.la


############################################################
## Variables for building libym_logic_zdd.la
############################################################

libym_logic_zdd_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_logic_zdd_la_SOURCES =

libym_logic_zdd_la_LIBADD = \
	base/libym_zdd_base.la

libym_logic_zdd_la_LDFLAGS =

nodist_EXTRA_libym_logic_zdd_la_SOURCES = dummy.cc
