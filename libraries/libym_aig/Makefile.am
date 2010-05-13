## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS = \
	ym_aig \
	. \
	tests

INCLUDES = \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libym_aig.la

libym_aig_la_LIBADD = \
	$(top_builddir)/libym_sat/libym_sat.la \
	$(top_builddir)/libym_npn/libym_npn.la \
	$(top_builddir)/libym_utils/libym_utils.la

libym_aig_la_LDFLAGS = 

libym_aig_la_SOURCES = \
	AigNode.cc \
	AigMgr.cc \
	AigMgrImpl.h \
	AigMgrImpl.cc \
	AigTemplate.h \
	AigTemplate.cc \
	FraigHash.h \
	FraigNode.h \
	FraigNode.cc \
	FraigMgrImpl.h \
	FraigMgrImpl.cc \
	FraigMgr.cc
