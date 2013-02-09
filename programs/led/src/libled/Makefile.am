## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 697 2007-05-28 17:10:53Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libled.la

libled_la_CPPFLAGS = \
	$(QT_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

libled_la_SOURCES = \
	Symbol.h \
	InputSymbol.h \
	OutputSymbol.h \
	GateSymbol.h \
	BufSymbol.h \
	NotSymbol.h \
	AndSymbol.h \
	OrSymbol.h \
	XorSymbol.h \
	Symbol.cc \
	GateObj.h \
	GateObj.cc


#libled_la_LDADD = \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
#	$(QT_LIBS)
