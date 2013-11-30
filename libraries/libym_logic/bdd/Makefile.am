## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS = \
	include \
	base \
	bmc \
	bmm \
	.

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic_bdd.la


############################################################
## Variables for building libym_logic_bdd.la
############################################################

libym_logic_bdd_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_logic_bdd_la_SOURCES =

libym_logic_bdd_la_LIBADD = \
	base/libym_bdd_base.la \
	bmc/libym_bdd_bmc.la \
	bmm/libym_bdd_bmm.la

libym_logic_bdd_la_LDFLAGS =

nodist_EXTRA_libym_logic_bdd_la_SOURCES = dummy.cc
