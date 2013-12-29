## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libec.la


# libec.la
libec_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(GMP_INCLUDES)

libec_la_SOURCES = \
	EnumCut.cc \
	EnumCut2.cc \
	BottomUp.cc \
	TopDown.cc \
	TopDown2.cc \
	ZddImp.cc \
	ZddImp2.cc \
	ZddTopDown.cc

libec_la_LIBADD =

libec_la_LDFLAGS =
