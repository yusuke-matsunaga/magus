## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## Variables for building targets
############################################################

noinst_LTLIBRARIES = libym_cell_ci.la


############################################################
## Variables for building libym_cell_ci.la
############################################################

libym_cell_ci_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_cell_ci_la_SOURCES = \
	CiBundle.h \
	CiBundle.cc \
	CiBus.h \
	CiBus.cc \
	CiCell.h \
	CiCell.cc \
	CiLibrary.h \
	CiLibrary.cc \
	CiPin.h \
	CiPin.cc \
	CiTiming.h \
	CiTiming.cc \
	restore.cc

libym_cell_ci_la_LIBADD =

libym_cell_ci_la_LDFLAGS =
