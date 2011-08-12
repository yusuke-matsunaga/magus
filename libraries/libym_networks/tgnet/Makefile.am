## Process this file with automake to produce Makefile.in

SUBDIRS = \
	blif \
	iscas89 \
	.

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_tgnet.la

libym_tgnet_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_tgnet_la_SOURCES = \
	NameHash.h \
	NameHash.cc \
	LogicMgr.h \
	LogicMgr.cc \
	TgNetwork.cc

libym_tgnet_la_LIBADD = \
	blif/libym_tgblifreader.la \
	iscas89/libym_tgiscas89reader.la

libym_tgnet_la_LDFLAGS =
