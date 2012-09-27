## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_gbmpy.la

############################################################
## libym_gbmpy.la
############################################################

libym_gbmpy_la_CPPFLAGS = \
	$(PYTHON_CPPFLAGS) \
	-I$(top_srcdir)/include

libym_gbmpy_la_SOURCES = \
	gbmpy.h \
	gbmpy_init.cc \
	gbmpy_Gbm.cc \
	gbmpy_GbmNode.cc \
	gbmpy_GbmNodeHandle.cc

libym_gbmpy_la_LIBADD =

libym_gbmpy_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
