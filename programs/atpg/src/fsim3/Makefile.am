## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_fsim3.la

libatpg_fsim3_la_SOURCES = \
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
	Fsim3.h \
	Fsim3.cc

libatpg_fsim3_la_LDFLAGS =

libatpg_fsim3_la_LIBADD = \
	$(YM_LIBS_LIBYM_TGNET) \
	$(YM_LIBS_LIBYM_LEXP) \
	$(YM_LIBS_LIBYM_UTILS)
