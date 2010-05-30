## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

YMLIBS_SRCDIR = $(top_srcdir)/../..
YMLIBS_BUILDDIR = $(top_builddir)/../..

INCLUDES = \
	-I$(YMLIBS_BUILDDIR) \
	-I$(YMLIBS_SRCDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_fsimx.la

libatpg_fsimx_la_SOURCES = \
	EqElem.h \
	SimNode.h \
	SimNode.cc \
	SnGate.h \
	SnGate.cc \
	SnInput.h \
	SnInput.cc \
	SnAnd.h \
	SnAnd.cc \
	SnOr.h \
	SnOr.cc \
	SnXor.h \
	SnXor.cc \
	SnCplx.h \
	SnCplx.cc \
	SimFFR.h \
	EventQ.h \
	EventQ.cc \
	FsimFault.h \
	FsimX.h \
	FsimX.cc

libatpg_fsimx_la_LDFLAGS =

libatpg_fsimx_la_LIBADD = \
	$(YMLIBS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMLIBS_BUILDDIR)/libraries/libym_utils/libym_utils.la
