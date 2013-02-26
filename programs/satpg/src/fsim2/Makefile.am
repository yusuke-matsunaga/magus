## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_fsim2.la

libatpg_fsim2_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libatpg_fsim2_la_SOURCES = \
	fsim2_nsdef.h \
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
	SimFFR.h \
	EventQ.h \
	EventQ.cc \
	FsimFault.h \
	Fsim2.h \
	Fsim2.cc

libatpg_fsim2_la_LIBADD =

libatpg_fsim2_la_LDFLAGS =
