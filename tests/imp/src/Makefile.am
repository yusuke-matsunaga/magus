## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = libym_imp bb .

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_PROGRAMS = \
	imp

imp_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/imp/include \
	-I$(top_srcdir)/imp/src/libym_imp \
	@GMP_INCLUDES@

imp_SOURCES = \
	imp.cc

imp_LDADD = \
	libym_imp/libym_imp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(POPT_LIBS) \
	$(GMP_LIBS)

imp_LDFLAGS =