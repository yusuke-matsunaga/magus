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
	matpg_utils/pkgIndex.tcl \
	matpg_utils/scripts/*.tcl \
	matpg_utils/scripts/tclIndex


############################################################
## 独自の make ルールの追加
############################################################

TCLSH = ${top_builddir}/old_matpg/src/matpg_tclsh/matpg_tclsh
SETUP = ${top_srcdir}/old_matpg/utils/tclpkg-setup.tcl
INSTALL = ${top_srcdir}/old_matpg/utils/tclpkg-install.tcl
PKGSRC = ${top_srcdir}/old_matpg/tclsrc

all-local :
	$(TCLSH) $(SETUP) $(PKGSRC) "{matpg_utils}"

install-data-local :
	$(TCLSH) $(INSTALL) $(PKGSRC) $(MATPG_LIBDIR) "{matpg_utils}"
