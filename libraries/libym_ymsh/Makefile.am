## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS = \
	ym_ymsh \
	.

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_ymsh.la

libym_ymsh_la_SOURCES = \
	Ymsh.cc \
	YmshImpl.h \
	YmshImpl.cc \
	YmshCmdTable.h \
	YmshCmdTable.cc \
	YmshCmd.cc \
	YmshObjTable.h \
	YmshObjTable.cc \
	SetCmd.h \
	SetCmd.cc \
	AliasCmd.h \
	AliasCmd.cc \
	ExitCmd.h \
	ExitCmd.cc \
	DirCmd.h \
	DirCmd.cc

libym_ymsh_la_LIBADD =

libym_ymsh_la_LDFLAGS = \
	-lreadline
