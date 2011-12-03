## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_logic_zdd.la


############################################################
## Variables for building libym_logic_zdd.la
############################################################

libym_logic_zdd_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(GMP_INCLUDES)

libym_logic_zdd_la_SOURCES = \
	CompTbl.h \
	CompTbl.cc \
	Zdd.cc \
	ZddEdge.h \
	ZddMgr.cc \
	ZddMgrImpl.h \
	ZddMgrImpl.cc \
	ZddNode.h \
	cap_op.cc \
	count.cc \
	support.cc

libym_logic_zdd_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_logic_zdd_la_LDFLAGS =

