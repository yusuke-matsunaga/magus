## Process this file with automake to produce Makefile.in


############################################################
## サブディレクトリの指定
############################################################

SUBDIRS =


############################################################
## clean ターゲットの指定
############################################################

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in


############################################################
## dist ターゲットの指定
############################################################

EXTRA_DIST = \
	seal_utils/pkgIndex.tcl \
	seal_utils/scripts/*.tcl \
	seal_utils/scripts/tclIndex


############################################################
## 独自の make ルールの追加
############################################################

TCLSH = ${top_builddir}/src/seal_tclsh
SETUP = ${top_srcdir}/utils/tclpkg-setup.tcl
INSTALL = ${top_srcdir}/utils/tclpkg-install.tcl
PKGSRC = ${top_srcdir}/tclsrc

all-local :
	$(TCLSH) $(SETUP) $(PKGSRC) "{seal_utils}"

install-data-local :
	$(TCLSH) $(INSTALL) $(PKGSRC) $(SEAL_LIBDIR) "{seal_utils}"
