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
	phf_fixed3 \
	phf_igu

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

phf_fixed3_SOURCES = \
	phf_fixed3.cc

phf_fixed3_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	-lrt \
	@POPT_LIBS@

phf_igu_SOURCES = \
	phf_igu.cc

phf_igu_LDADD = \
	../libphf/libphf.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@
