
/// @file myzcat.cc
/// @brief zcat を真似てみた．
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"
#include <sys/cdefs.h>
#include "ymutils/zstream.h"


#define GZIP_MAGIC0	0x1F
#define GZIP_MAGIC1	0x8B
#define GZIP_OMAGIC1	0x9E

#define GZIP_TIMESTAMP	(off_t)4
#define GZIP_ORIGNAME	(off_t)10

#define HEAD_CRC	0x02
#define EXTRA_FIELD	0x04
#define ORIG_NAME	0x08
#define COMMENT		0x10


BEGIN_NONAMESPACE

int qflag;			/* quiet mode */
int exit_value = 0;		/* exit value */

// maybe print a warning
void
maybe_warn(const char *fmt, ...)
{
  va_list ap;

  if ( qflag == 0 ) {
    va_start(ap, fmt);
    vwarn(fmt, ap);
    va_end(ap);
  }
  if ( exit_value == 0 ) {
    exit_value = 1;
  }
}

// ... without an errno.
void
maybe_warnx(const char *fmt, ...)
{
  va_list ap;

  if ( qflag == 0 ) {
    va_start(ap, fmt);
    vwarnx(fmt, ap);
    va_end(ap);
  }
  if ( exit_value == 0 ) {
    exit_value = 1;
  }
}

/* maybe print an error */
void
maybe_err(const char *fmt, ...)
{
  va_list ap;

  if ( qflag == 0 ) {
    va_start(ap, fmt);
    vwarn(fmt, ap);
    va_end(ap);
  }
  exit(2);
}


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
  z_stream z;
  char *outbufp;
  char *inbufp;
  off_t out_tot = -1;
  off_t in_tot = 0;
  uint32_t out_sub_tot = 0;
  enum {
    GZSTATE_MAGIC0,
    GZSTATE_MAGIC1,
    GZSTATE_METHOD,
    GZSTATE_FLAGS,
    GZSTATE_SKIPPING,
    GZSTATE_EXTRA,
    GZSTATE_EXTRA2,
    GZSTATE_EXTRA3,
    GZSTATE_ORIGNAME,
    GZSTATE_COMMENT,
    GZSTATE_HEAD_CRC1,
    GZSTATE_HEAD_CRC2,
    GZSTATE_INIT,
    GZSTATE_READ,
    GZSTATE_CRC,
    GZSTATE_LEN,
  } state = GZSTATE_MAGIC0;
  int flags = 0;
  int skip_count = 0;
  int error = Z_STREAM_ERROR;
  bool done_reading = false;
  uLong crc = 0;
  ssize_t wr;
  bool needmore = false;

#define ADVANCE()       { z.next_in++; z.avail_in--; }

  // 入出力バッファを確保する．
  if ( (outbufp = malloc(BUFLEN)) == NULL ) {
    maybe_err("malloc failed");
    goto out2;
  }
  if ( (inbufp = malloc(BUFLEN)) == NULL ) {
    maybe_err("malloc failed");
    goto out1;
  }

  // z_stream 構造体の初期化
  // 少し面倒なのは先読みした部分が (pre[0] -- pre[prelen - 1])
  // に入っているところ．
  memset(&z, 0, sizeof z);
  z.avail_in = prelen;
  z.next_in = (unsigned char *)pre;
  z.avail_out = BUFLEN;
  z.next_out = (unsigned char *)outbufp;
  z.zalloc = NULL;
  z.zfree = NULL;
  z.opaque = 0;

  in_tot = prelen;
  out_tot = 0;

  for ( ; ; ) {
    // 必要ならバッファに読み込む．
    if ( (z.avail_in == 0 || needmore) && !done_reading ) {
      if ( z.avail_in > 0 ) {
	memmove(inbufp, z.next_in, z.avail_in);
      }
      z.next_in = (unsigned char *)inbufp;
      ssize_t in_size = read(in, z.next_in + z.avail_in, BUFLEN - z.avail_in);
      if ( in_size == -1 ) {
	maybe_warn("failed to read stdin");
	goto stop_and_fail;
      }
      else if ( in_size == 0 ) {
	done_reading = 1;
      }

      z.avail_in += in_size;
      needmore = 0;

      in_tot += in_size;
    }
    if ( z.avail_in == 0 ) {
      if ( done_reading && state != GZSTATE_MAGIC0 ) {
	maybe_warnx("%s: unexpected end of file",
		    filename);
	goto stop_and_fail;
      }
      goto stop;
    }

    // 状態にしたがって処理を行う．
    switch ( state ) {
    case GZSTATE_MAGIC0:
      if ( z.next_in[0] != GZIP_MAGIC0 ) {
	if ( in_tot > 0 ) {
	  maybe_warnx("%s: trailing garbage "
		      "ignored", filename);
	  goto stop;
	}
	maybe_warnx("input not gziped (MAGIC0)");
	goto stop_and_fail;
      }
      ADVANCE();

      state = GZSTATE_MAGIC1;
      out_sub_tot = 0;
      crc = crc32(0L, Z_NULL, 0);
      break;

    case GZSTATE_MAGIC1:
      if ( z.next_in[0] != GZIP_MAGIC1 &&
	   z.next_in[0] != GZIP_OMAGIC1) {
	maybe_warnx("input not gziped (MAGIC1)");
	goto stop_and_fail;
      }
      ADVANCE();

      state = GZSTATE_METHOD;
      break;

    case GZSTATE_METHOD:
      if ( z.next_in[0] != Z_DEFLATED ) {
	maybe_warnx("unknown compression method");
	goto stop_and_fail;
      }
      ADVANCE();

      state = GZSTATE_FLAGS;
      break;

    case GZSTATE_FLAGS:
      flags = z.next_in[0];
      ADVANCE();

      skip_count = 6;
      state = GZSTATE_SKIPPING;
      break;

    case GZSTATE_SKIPPING:
      if ( skip_count > 0 ) {
	-- skip_count;
	ADVANCE();
      }
      else {
	state = GZSTATE_EXTRA;
      }
      break;

    case GZSTATE_EXTRA:
      if ( (flags & EXTRA_FIELD) == 0 ) {
	state = GZSTATE_ORIGNAME;
	break;
      }
      skip_count = z.next_in[0];
      ADVANCE();

      state = GZSTATE_EXTRA2;
      break;

    case GZSTATE_EXTRA2:
      skip_count |= (z.next_in[0] << 8);
      ADVANCE();

      state = GZSTATE_EXTRA3;
      break;

    case GZSTATE_EXTRA3:
      if ( skip_count > 0 ) {
	-- skip_count;
	ADVANCE();
      }
      else {
	state = GZSTATE_ORIGNAME;
      }
      break;

    case GZSTATE_ORIGNAME:
      if ( (flags & ORIG_NAME) == 0 ) {
	state = GZSTATE_COMMENT;
	break;
      }
      if ( z.next_in[0] == 0 ) {
	state = GZSTATE_COMMENT;
      }
      ADVANCE();
      break;

    case GZSTATE_COMMENT:
      if ( (flags & COMMENT) == 0 ) {
	state = GZSTATE_HEAD_CRC1;
	break;
      }
      if ( z.next_in[0] == 0 ) {
	state = GZSTATE_HEAD_CRC1;
      }
      ADVANCE();
      break;

    case GZSTATE_HEAD_CRC1:
      if ( flags & HEAD_CRC ) {
	skip_count = 2;
      }
      else {
	skip_count = 0;
      }
      state = GZSTATE_HEAD_CRC2;
      break;

    case GZSTATE_HEAD_CRC2:
      if ( skip_count > 0 ) {
	-- skip_count;
	ADVANCE();
      }
      else {
	state = GZSTATE_INIT;
      }
      break;

    case GZSTATE_INIT:
      if ( inflateInit2(&z, -MAX_WBITS) != Z_OK ) {
	maybe_warnx("failed to inflateInit");
	goto stop_and_fail;
      }
      state = GZSTATE_READ;
      break;

    case GZSTATE_READ:
      error = inflate(&z, Z_FINISH);
      switch ( error ) {
	/* Z_BUF_ERROR goes with Z_FINISH... */
      case Z_BUF_ERROR:
	if ( z.avail_out > 0 && !done_reading ) {
	  continue;
	}

      case Z_STREAM_END:
      case Z_OK:
	break;

      case Z_NEED_DICT:
	maybe_warnx("Z_NEED_DICT error");
	goto stop_and_fail;

      case Z_DATA_ERROR:
	maybe_warnx("data stream error");
	goto stop_and_fail;

      case Z_STREAM_ERROR:
	maybe_warnx("internal stream error");
	goto stop_and_fail;

      case Z_MEM_ERROR:
	maybe_warnx("memory allocation error");
	goto stop_and_fail;

      default:
	maybe_warn("unknown error from inflate(): %d",
		   error);
      }
      wr = BUFLEN - z.avail_out;
      if ( wr != 0 ) {
	crc = crc32(crc, (const Bytef *)outbufp, (unsigned)wr);
	/* don't write anything with -t */
	if ( !tflag ) {
	  int act_wr = write(out, outbufp, wr);
	  if ( act_wr != wr ) {
	    maybe_warn("error writing to output");
	    goto stop_and_fail;
	  }
	}

	out_tot += wr;
	out_sub_tot += wr;
      }

      if ( error == Z_STREAM_END ) {
	inflateEnd(&z);
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
