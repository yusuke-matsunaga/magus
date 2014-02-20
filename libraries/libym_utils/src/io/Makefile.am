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

EXTRA_DIST =


############################################################
## build ターゲットの指定
############################################################

noinst_LTLIBRARIES = libio.la


############################################################
## 汎用のプリプロセッサ用フラグの指定
############################################################

AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_builddir)/include


############################################################
## libio.la 用の変数の定義
############################################################

libio_la_SOURCES = \
	ZCoder.h \
	ZCoder.cc \
	ZDecoder.h \
	ZDecoder.cc \
	FileBuff.h \
	FileBuff.cc \
	FileCoder.cc \
	FileDecoder.cc \
	FileIDO.cc \
	FileODO.cc \
	IDO.cc \
	ODO.cc \
	ThruCoder.h \
	ThruCoder.cc \
	ThruDecoder.h \
	ThruDecoder.cc

libio_la_LIBADD =

libio_la_LDFLAGS =


if YM_HAVE_ZLIB
libio_la_SOURCES += \
	zstream.h \
	zstream.cc \
	GzCoder.h \
	GzCoder.cc \
	GzDecoder.h \
	GzDecoder.cc

libio_la_LDFLAGS += \
	-lz
endif

if YM_HAVE_BZLIB
libio_la_SOURCES += \
	BzEngine.h \
	BzCoder.h \
	BzCoder.cc \
	BzDecoder.h \
	BzDecoder.cc

libio_la_LDFLAGS += \
	-lbz2
endif

if YM_HAVE_LZMA
libio_la_SOURCES += \
	LzmaEngine.h \
	LzmaCoder.h \
	LzmaCoder.cc \
	LzmaDecoder.h \
	LzmaDecoder.cc

libio_la_LDFLAGS += \
	-llzma
endif
