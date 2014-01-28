## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 997 2007-09-07 09:58:29Z matsunaga $

SUBDIRS = \
	ym_gds \
	. \
	tests

CLEANFILES = \
	gds_grammer.cc \
	gds_grammer.hh \
	y.output \
	gds_grammer.output

MAINTAINERCLEANFILES = Makefile.in


AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

AM_YFLAGS = -d -v


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_gds.la


############################################################
## Variables for building libym_gds.la
############################################################

libym_gds_la_SOURCES = \
	gds_grammer.yy \
	Msg.cc \
	GdsDumper.cc \
	GdsParser.cc \
	GdsRecTable.h \
	GdsRecTable.cc \
	GdsRecMgr.cc \
	GdsRecord.cc \
	GdsScanner.cc \
	GdsAref.h \
	GdsAref.cc \
	GdsBoundary.h \
	GdsBoundary.cc \
	GdsBox.h \
	GdsBox.cc \
	GdsData.cc \
	GdsElement.cc \
	GdsFormat.cc \
	GdsNode.h \
	GdsNode.cc \
	GdsRefBase.h \
	GdsRefBase.cc \
	GdsPath.h \
	GdsPath.cc \
	GdsSref.h \
	GdsSref.cc \
	GdsStruct.cc \
	GdsText.h \
	GdsText.cc

libym_gds_la_LIBADD =
#	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_gds_la_LDFLAGS =
