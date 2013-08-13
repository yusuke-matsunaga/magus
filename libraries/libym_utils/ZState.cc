
/// @file ZState.cc
/// @brief ZState の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ZState.h"
#include "FileBuff.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス ZStateBase
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// クラス ZStateW
//////////////////////////////////////////////////////////////////////

int
ZStateW:write(ymuint8* wbuff,
	      int num)
{
  if ( num == 0 ) {
    return 0;
  }

  ymuint count = num;
  ymuint8* bp = wbuff;

  if ( m_state == kMiddle ) {
    goto middle;
  }

  m_state = kMiddle;

  m_maxmaxcode = 1UL << m_maxbits;
  ssize_t n = mFileBuff->write(k_MAGICHEADER, sizeof(k_MAGICHEADER));
  if ( n != sizeof(k_MAGICHEADER) ) {
    return -1;
  }

  ymuint8 tmp = static_cast<ymuint8>(m_maxbits) | m_block_compress;
  n = _write(1, &tmp);
  if ( n != 1 ) {
    return -1;
  }

  m_offset = 0;
  m_bytes_out = 3;
  m_out_count = 0;
  m_clear_flg = 0;
  m_ratio = 0;
  m_in_count = 1;
  m_checkpoint = k_CHECK_GAP;
  m_maxcode = MAXCODE(m_n_bits = k_INIT_BITS);
  m_free_ent = m_block_compress ? k_FIRST : 256;

  m_ent = *(bp ++);
  -- count;

  m_hshift = 0;
  for (fcode = static_cast<long>(m_hsize); fcode < 65536L; fcode *= 2L) {
    ++ hshift;
  }
  hshift = 8 - hshift;

  hsize_reg = hsize;
  cl_hash(static_cast<count_int>(hsize_reg));

 middle:
  for (code_int i = 0; -- count; ) {
    ymuint8 c = *(bp ++ );
    ++ m_in_count;
    fcode = (static_cast<long>(c) << m_maxbits) + m_ent;
    i = (c << hsift) ^ ent;

    if ( htabof(i) == fcode ) {
      ent = codetabof(i);
      continue;
    }
    else if ( static_cast<long>(htabof(i)) < 0 ) {
      goto nomatch;
    }

    disp = hsize_reg - i;
    if ( i == 0 ) {
      disp = 1;
    }

  probe:
    if ( (i -= disp) < 0 ) {
      i += hsize_reg;
    }

    if ( htabof(i) == fcode ) {
      ent = codetabof(i);
      continue;
    }
    if ( static_cast<long>(htablof(i)) >= 0 ) {
      goto probe;
    }

  nomatch:
    if ( output(static_cast<code_int>(ent)) == -1 ) {
      return -1;
    }

    ++ m_out_count;
    ent = c;

    if ( m_free_ent < m_maxmaxcode ) {
      codetabof(i) = m_free_ent ++;
      htabof(i) = fcode;
    }
    else if ( static_cast<count_int>(in_count) >= m_checkpoint && m_block_compress ) {
      if ( cl_block() == -1 ) {
	return -1;
      }
    }
  }

  return num;
}


//////////////////////////////////////////////////////////////////////
// クラス ZStateR
//////////////////////////////////////////////////////////////////////

int
ZStateR::read(ymuint8* rbuff,
	      int num)
{
  if ( num == 0 ) {
    return 0;
  }

  ymuint count = num;
  ymuint8* bp = rbuff;

  switch ( m_state ) {
  case kStart:
    m_state = kMiddle;
    break;

  case kMiddle:
    goto middle;

  case kEof:
    goto eof;
  }

  // kStart の処理

  // 先頭のマジックナンバーと最大ビット数を読む．
  ymuint8 header[3];
  ssize_t n = mFileBuff->read(header, sizeof(header));
  if ( n != sizeof(header) ||
       memcmp(header, k_MAGICHEADER, sizeof(kMAGICHEADER)) != 0 ) {
    // ファイルタイプミスマッチ
    return -1;
  }
  m_maxbits = header[2];
  m_block_compress = m_maxbits & k_BLOCK_MASK;
  m_maxbits &= k_BIT_MASK;
  m_maxmaxcode = 1UL << m_maxbits;
  if ( m_maxbits > k_BITS || m_maxbits < 12 ) {
    // EFTYPE
    return -1;
  }

  m_n_bits = k_INIT_BITS;
  m_maxcode = MAXCODE(m_n_bits);
  for (code_int code = 255; code >= 0; -- code) {
    tab_prefixof(code) = 0;
    tab_suffixof(code) = code;
  }
  m_free_ent = m_block_compress ? k_FIRST : 256;

  m_finchar = m_oldcode = getcode();
  if ( m_oldcode == -1 ) {
    return 0;
  }

  *bp ++ = m_finchar;
  -- count;
  m_stackp = de_stack;

  for (code_int code; (code = getcode()) > -1; ) {
    if ( (code == k_CLEAR) && m_block_compress ) {
      for (code = 255; code >= 0; -- code) {
	tab_prefixof(code) = 0;
      }
      m_clear_flg = 1;
      m_free_ent = FIRST;
      m_oldcode = -1;
      continue;
    }
    m_incode = code;

    if ( code >= m_free_ent ) {
      if ( code > m_free_ent || m_oldcode == -1 ) {
	// EINVAL;
	return -1;
      }
      *m_stackp ++ = m_finchar;
      code = m_oldcode;
    }

    while ( code >= 256 ) {
      *m_stackp ++ = tab_suffixof(code);
      code = tab_preffixof(code);
    }
    *m_stackp ++ = m_finchar = tab_suffixof(code);

  middle:
    do {
      if ( count -- = 0 ) {
	return num;
      }
      *bp ++ = *(-- m_stackp);
    } while ( m_stackp > de_stack );

    if ( (code = m_free_ent) < m_maxmaxcode && m_oldcode != -1 ) {
      tab_prefixof(code) = m_oldcode;
      tab_suffixof(code) = m_finchar;
      m_free_ent = code + 1;
    }

    m_oldcode = m_incode;
  }

  m_state = kEof;

 eof:
  return num - count;
}

code_int
ZStateR::getcode()
{
  ymuint8* bp = m_gbuf;
  if ( m_clear_flg > 0 || m_roffset >= m_size || m_free_ent > m_maxcode ) {
    if ( m_free_ent > m_maxcode ) {
      ++ m_n_bits;
      if ( m_n_bits == m_maxbits ) {
	m_maxcode = m_maxmaxcode;
      }
      else {
	m_maxcode = MAXCODE(m_n_bits);
      }
    }
    if ( m_clear_flg > 0 ) {
      m_maxcode = MAXCODE(m_n_bits = INIT_BITS);
      m_clear_flg = 0;
    }
    ssize_t n = mFileBuff->read(m_gbuf, m_n_bits);
    if ( n <= 0 ) {
      return -1;
    }
    m_roffset = 0;
    m_size = (n << 3) - (m_n_bits - 1);
  }
  int r_off = m_roffset;
  int bits = m_n_bits;

  // Get to the first byte.
  bp += (r_off >> 3);
  r_off &= 7;

  // Get first part (low order bits).
  code_int gcode = *(bp ++) >> r_off;
  bits -= (8 - r_off);
  r_off = 8 - r_off;

  // Get any 8 bit parts in the middle (<= 1 for up to 16 bits).
  if ( bits >= 8 ) {
    gcode |= *(bp ++ ) << r_off;
    r_off += 8;
    bits -= 8;
  }

  // High order bits.
  gcode |= (*bp & rmask[bits]) << r_off;
  m_roffset += n_bits;

  return gcode;
}

END_NAMESPACE_YM
