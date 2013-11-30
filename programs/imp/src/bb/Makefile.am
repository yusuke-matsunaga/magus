## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1417 2008-05-06 18:12:40Z matsunaga $

SUBDIRS = \
	include \
	parser \
	pt \
	.

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	bb

bb_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/bb/include

bb_SOURCES = \
	bb.cc
bb_LDADD = \
	../libym_imp/libym_imp.la \
	parser/libbb_parser.la \
	pt/libbb_pt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la
