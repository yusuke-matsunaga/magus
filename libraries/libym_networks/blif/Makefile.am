## Process this file with automake to produce Makefile.in

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_blif.la


############################################################
## Variables for building libym_blif.la
############################################################

libym_blif_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_blif_la_SOURCES = \
	blif_nsdef.h \
	BlifDic.h \
	BlifDic.cc \
	BlifScanner.h \
	BlifScanner.cc \
	IdHash.h \
	IdHash.cc \
	BlifParser.h \
	BlifParser.cc \
	BlifParserImpl.h \
	BlifParserImpl.cc \
	BlifHandler.h \
	BlifHandler.cc \
	BlifNetwork.h \
	BlifNetwork.cc \
	BlifNetworkHandler.h \
	BlifNetworkHandler.cc \
	BlifNetworkReader.h \
	BlifNetworkReader.cc

libym_blif_la_LIBADD =

libym_blif_la_LDFLAGS =
