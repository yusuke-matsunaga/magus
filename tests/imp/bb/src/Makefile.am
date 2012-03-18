## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 697 2007-05-28 17:10:53Z matsunaga $

SUBDIRS = \
	parser \
	pt \
	.

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/imp \
	-I$(top_srcdir)/imp/bb/include

bin_PROGRAMS = \
	bb

bb_SOURCES = \
	bb.cc
bb_LDADD = \
	$(top_builddir)/imp/libym_imp.la \
	parser/libbb_parser.la \
	pt/libbb_pt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la
