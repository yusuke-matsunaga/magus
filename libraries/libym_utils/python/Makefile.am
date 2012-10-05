## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_utils_python.la

############################################################
## libym_utils_python.la
############################################################

libym_utils_python_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

libym_utils_python_la_SOURCES = \
	ympython.cc \
	pyutils_init.cc \
	pyutils_USTime.cc \
	pyutils_StopWatch.cc \
	pyutils_MStopWatch.cc \
	pyutils_RandGen.cc \
	pyutils_RandPermGen.cc \
	pyutils_RandCombiGen.cc

libym_utils_python_la_LIBADD =

libym_utils_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
