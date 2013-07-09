## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libigf_python.la

############################################################
## libigf_python.la
############################################################

libigf_python_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	$(PYTHON_CPPFLAGS)

libigf_python_la_SOURCES = \
	pyigf_FuncVect.cc \
	pyigf_RegVect.cc \
	pyigf_RvMgr.cc

libigf_python_la_LIBADD =

libigf_python_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
