## Process this file with automake to produce Makefile.in

SUBDIRS =

MAINTAINERCLANFILES = Makefiles.in

bin_PROGRAMS = pysatpg

pysatpg_CPPFLAGS = \
	$(PYTHON_CPPFLAGS) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

pysatpg_SOURCES = \
	pysatpg.cc \
	pysatpg_Val3.cc \
	pysatpg_FaultMgr.cc \
	pysatpg_FaultStatus.cc \
	pysatpg_TpgFault.cc \
	pysatpg_TestVector.cc \
	pysatpg_TvMgr.cc \
	pysatpg_AtpgMgr.cc


pysatpg_LDFLAGS = \
	$(PYTHON_LDFLAGS)

pysatpg_LDADD = \
	$(top_builddir)/src/main/libatpg_main.la \
	$(top_builddir)/src/rtpg/libatpg_rtpg.la \
	$(top_builddir)/src/dtpg/libatpg_dtpg.la \
	$(top_builddir)/src/minpat/libatpg_minpat.la \
	$(top_builddir)/src/fsim/libatpg_fsim.la \
	$(top_builddir)/src/fsim_old/libatpg_fsim_old.la \
	$(top_builddir)/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_networks/libym_networks.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_logic/libym_logic.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/python/libym_utils_python.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la
