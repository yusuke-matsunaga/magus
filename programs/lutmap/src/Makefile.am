## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

AM_CXXFLAGS = \
	-DLUTMAP_LIBDIR='"$(LUTMAP_LIBDIR)"' \
	-DLUTMAP_DATADIR='"$(LUTMAP_DATADIR)"' \
	-DLUTMAP_BINDIR='"$(LUTMAP_BINDIR)"'

INCLUDES = \
	-I$(YMLIBS_BUILDDIR) \
	-I$(YMLIBS_SRCDIR)/include

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	lutmap

lutmap_SOURCES = \
	lutmap.cc

lutmap_LDADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_lutmap/libym_lutmap.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_utils/libym_utils.la
