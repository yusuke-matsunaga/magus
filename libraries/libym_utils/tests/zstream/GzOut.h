#ifndef GZ_H
#define GZ_H

/// @file GZ.h
/// @brief GZ のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"

#if defined(HAVE_ZLIB)
#include <zlib.h>
#else
#error "\"ym_utils/GZ.h\" requires <zlib.h>"
#endif
//#include <sys/cdefs.h>
//#include "ymutils/zstream.h"


#define GZIP_MAGIC0	0x1F
#define GZIP_MAGIC1	0x8B
#define GZIP_OMAGIC1	0x9E

#define GZIP_TIMESTAMP	(off_t)4
#define GZIP_ORIGNAME	(off_t)10

#define HEAD_CRC	0x02
#define EXTRA_FIELD	0x04
#define ORIG_NAME	0x08
#define COMMENT		0x10


BEGIN_NAMESPACE_YM

int qflag;			/* quiet mode */
int exit_value = 0;		/* exit value */


//////////////////////////////////////////////////////////////////////
/// @class GZ GZ.h "GZ.h"
/// @brief zlib を利用した gzip エンジン
//////////////////////////////////////////////////////////////////////
class GZ
{
public:


public:
  //////////////////////////////////////////////////////////////////////
  // 公開インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 圧縮する．

  /// @brief 解凍する．

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};


// uncompress input to output then close the input.  return the
// uncompressed size written, and put the compressed sized read
// into `*gsizep'.
off_t
gz_uncompress(int in,
	      int out,
	      char *pre,
	      size_t prelen,
	      off_t *gsizep,
	      const char *filename)
{
  off_t out_tot = -1;
  off_t in_tot = 0;
  uint32_t out_sub_tot = 0;
  int error = Z_STREAM_ERROR;
  bool done_reading = false;
  bool needmore = false;


  // ヘッダ部分を解釈する．

  // 最初のバイトは CM(Compression Method)
  ymuint8 cm = in.read_8();
  if ( cm != Z_DEFLATED ) {
    maybe_err("unknown cpression method");
  }

  // 2番めのバイトは FLG(FLaGs)
  ymuint8 flags = in.read_8();

  // 3 - 6 番めのバイトは MTIME(Modification TIME)
  // 7 番めのバイトは XFL (eXtra FLags)
  // 8 番めのバイトは OS(Operationg Systeam)
  // 全部無視
  ymuint32 mtime = in.read_32();
  ymuint8 xfl = in.read_8();
  ymuint8 os = in.read_8();

  if ( flags & EXTRA_FIELD ) {
    // EXTRA_FIELD がセットされていたら最初の2バイトにそのサイズが書いてある．
    ymuint16 xlen = in.read_16();
    // その分を読み飛ばす．
    for (ymuint i = 0; i < xlen; ++ i) {
      (void) in.read_8();
    }
  }

  if ( flags & ORIG_NAME ) {
    // ORIG_NAME がセットされていたら0終端の文字列が書いてある．
    ymuint8 c;
    while ( (c = in.read_8()) != '\0' ) ;
  }

  if ( flags & COMMENT ) {
    // COMMENT がセットされていたら0終端の文字列が書いてある．
    ymuint8 c;
    while ( (c = in.read_8()) != '\0' ) ;
  }

  if ( flags & HEAD_CRC ) {
    // HEAD_CRC がセットされていたら2バイトのCRCコードが書いてある．
    (void) in.read_16();
  }

  // 以降が圧縮されたデータ本体となる．
  zstream zs;

  if ( zs.inflateInit2(- MAX_WBITS) != Z_OK ) {
    maybe_err("failed to inflateInit2");
  }

  ymuint8 outbuf = new ymuint8(4096);

  zs.set_outbuf(outbuf, 4096);

  ymuint32 crc = 0;
  for (bool data_end = false; !data_end; ) {
    zs.set_inbuf(in.next_ptr(), in.avail_size());
    int stat = zs.inflate(Z_FINISH);
    switch ( state ) {
    case Z_BUF_ERROR:
      // Z_BUF_ERROR goes with Z_FINISH...
      if ( zs.avail_out() > 0 && !done_reading ) {
	continue;
      }

    case Z_STREAM_END:
      zs.inflateEnd();
      data_end = true;
      break;

    case Z_OK:
      break;

    case Z_NEED_DICT:
      maybe_err("Z_NEED_DICT error");
      break;

    case Z_DATA_ERROR:
      maybe_err("data stream error");
      break;

    case Z_STREAM_ERROR:
      maybe_err("internal stream error");
      break;

    case Z_MEM_ERROR:
      maybe_err("memory allocation error");
      break;

    default:
      maybe_err("unknown error from inflate(): %d", status);
    }

    wr = BUFLEN - z.avail_out;
    if ( wr != 0 ) {
      crc = crc32(crc, (const Bytef *)outbufp, (unsigned)wr);
      int act_wr = write(out, outbufp, wr);
      if ( act_wr != wr ) {
	maybe_err("error writing to output");
      }

      out_tot += wr;
      out_sub_tot += wr;
    }

    if ( error == Z_STREAM_END ) {
      state = GZSTATE_CRC;
    }

      z.next_out = (unsigned char *)outbufp;
      z.avail_out = BUFLEN;

      break;

    case GZSTATE_CRC:
      {
	if ( z.avail_in < 4 ) {
	  if ( !done_reading ) {
	    needmore = 1;
	    continue;
	  }
	  maybe_warnx("truncated input");
	  goto stop_and_fail;
	}
	uLong origcrc = ((unsigned)z.next_in[0] & 0xff) |
	  ((unsigned)z.next_in[1] & 0xff) << 8 |
	  ((unsigned)z.next_in[2] & 0xff) << 16 |
	  ((unsigned)z.next_in[3] & 0xff) << 24;
	if ( origcrc != crc ) {
	  maybe_warnx("invalid compressed"
		      " data--crc error");
	  goto stop_and_fail;
	}
      }

      z.avail_in -= 4;
      z.next_in += 4;

      if ( !z.avail_in && done_reading ) {
	goto stop;
      }

      state = GZSTATE_LEN;
      break;

    case GZSTATE_LEN:
      {
	if ( z.avail_in < 4 ) {
	  if ( !done_reading ) {
	    needmore = 1;
	    continue;
	  }
	  maybe_warnx("truncated input");
	  goto stop_and_fail;
	}
	uLong origlen = ((unsigned)z.next_in[0] & 0xff) |
	  ((unsigned)z.next_in[1] & 0xff) << 8 |
	  ((unsigned)z.next_in[2] & 0xff) << 16 |
	  ((unsigned)z.next_in[3] & 0xff) << 24;

	if ( origlen != out_sub_tot ) {
	  maybe_warnx("invalid compressed"
		      " data--length error");
	  goto stop_and_fail;
	}
      }

      z.avail_in -= 4;
      z.next_in += 4;

      if ( error < 0 ) {
	maybe_warnx("decompression error");
	goto stop_and_fail;
      }
      state = GZSTATE_MAGIC0;
      break;
    }
    continue;

  stop_and_fail:
    out_tot = -1;

  stop:
    break;
  }

  if ( state > GZSTATE_INIT ) {
    inflateEnd(&z);
  }

  free(inbufp);

 out1:
  free(outbufp);

 out2:
  if ( gsizep ) {
    *gsizep = in_tot;
  }

  return (out_tot);
}

END_NONAMESPACE
