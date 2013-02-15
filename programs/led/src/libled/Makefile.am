## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 697 2007-05-28 17:10:53Z matsunaga $

include $(top_srcdir)/../../etc/autotroll.mk

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
	DrawObj.h \
	DrawObj.cc \
	Symbol.h \
	Symbol.cc \
	GateMgr.cc \
	GateObj.cc \
	NetworkView.cc \
	NetworkView.moc.cc


BUILT_SOURCES = \
	NetworkView.moc.cc

NetworkView.moc.cc : ../../include/NetworkView.h

#libled_la_LDADD = \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
#	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
#	$(QT_LIBS)

