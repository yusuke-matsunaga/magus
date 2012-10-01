## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_logicpy.la

############################################################
## libym_logicpy.la
############################################################

libym_logicpy_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

libym_logicpy_la_SOURCES = \
	logicpy.h \
	logicpy_init.cc

libym_logicpy_la_LIBADD =

libym_logicpy_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
