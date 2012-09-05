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
	BlifDic.h \
	BlifDic.cc \
	BlifScanner.h \
	BlifScanner.cc \
	IdCell.h \
	IdHash.h \
	IdHash.cc \
	BlifParser.h \
	BlifParser.cc \
	BlifHandler.h \
	BlifHandler.cc \
	BlifNetwork.cc \
	BlifNetworkImpl.h \
	BlifNetworkImpl.cc \
	BlifNetworkHandler.h \
	BlifNetworkHandler.cc

libym_blif_la_LIBADD =

libym_blif_la_LDFLAGS =
