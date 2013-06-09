## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

AM_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	@POPT_INCLUDES@

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	igf \
	migf \
	igf2 \
	var_analyzer \
	phf \
	phf_fixed \
	phf2 \
	phf3

igf_SOURCES = \
	igf.cc

igf_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lrt \
	@POPT_LIBS@

migf_SOURCES = \
	migf.cc

migf_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lrt \
	@POPT_LIBS@

igf2_SOURCES = \
	igf2.cc

igf2_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lrt \
	@POPT_LIBS@

var_analyzer_SOURCES = \
	var_analyzer.cc

var_analyzer_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lrt \
	@POPT_LIBS@

phf_SOURCES = \
	phf.cc

phf_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

phf_fixed_SOURCES = \
	phf_fixed.cc

phf_fixed_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lrt \
	@POPT_LIBS@

phf2_SOURCES = \
	phf2.cc \
	Phf2Node.h \
	Phf2Edge.h \
	Phf2Gen.h \
	Phf2Gen.cc

phf2_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@

phf3_SOURCES = \
	phf3.cc \
	Phf3Node.h \
	Phf3Edge.h \
	Phf3Gen.h \
	Phf3Gen.cc

phf3_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@
