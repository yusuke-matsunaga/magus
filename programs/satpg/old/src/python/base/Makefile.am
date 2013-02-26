## Process this file with automake to produce Makefile.in

SUBDIRS =


noinst_LTLIBRARIES = libym_atpg_pybase.la

############################################################
## libym_atpg_pybase.la
############################################################

libym_atpg_pybase_la_CPPFLAGS = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	$(PYTHON_CPPFLAGS)

libym_atpg_pybase_la_SOURCES = \
	pybase_init.cc \
	pybase_SaFault.cc \
	pybase_FaultMgr.cc \
	pybase_TestVector.cc

libym_atpg_pybase_la_LIBADD =

libym_atpg_pybase_la_LDFLAGS = \
	$(PYTHON_LDFLAGS)
