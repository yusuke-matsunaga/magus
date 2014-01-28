## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic_aig.la


############################################################
## Variables for building libym_aig.la
############################################################

libym_logic_aig_la_SOURCES = \
	AigNode.h \
	AigNode.cc \
	AigMgr.cc \
	AigMgrImpl.h \
	AigMgrImpl.cc \
	AigSatMgr.cc

#	AigSatData.h \
#	AigSatMgr.cc \
#	AigTemplate.h \
#	AigTemplate.cc

libym_logic_aig_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_logic_aig_la_LDFLAGS =
