
/// @file ZCoder.cc
/// @brief ZCoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ZCoder.h"


BEGIN_NAMESPACE_YM_COMPCOMMON

//////////////////////////////////////////////////////////////////////
// クラス ZCoder
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
ZCoder::ZCoder()
{
}

// @brief デストラクタ
ZCoder::~ZCoder()
{
  close();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @param[in] mode ファイル作成用のモード
// @param[in] level 圧縮レベル
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルに対する書き込み許可がない．
bool
ZCoder::open(const char* filename,
	     mode_t mode,
	     ymuint level)
{
  m_state = kStart;

  m_maxbits = level ? level : k_BITS;	/* User settable max # bits/code. */
  m_maxmaxcode = 1L << m_maxbits;	/* Should NEVER generate this code. */
  m_free_ent = 0;			/* First unused entry. */
  m_block_compress = k_BLOCK_MASK;
  m_clear_flg = 0;

  m_hsize = k_HSIZE;			/* For dynamic table sizing. */
  m_ratio = 0;
  m_checkpoint = k_CHECK_GAP;
  m_in_count = 1;			/* Length of input. */
  m_out_count = 0;			/* # of codes output (for debugging). */

  return mBuff.open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

// @brief ファイルを閉じる．
void
ZCoder::close()
{
  if ( is_ready() ) {
    if ( output(m_ent) == -1 ) {
      goto end;
    }

    ++ m_out_count;

    if ( output(static_cast<code_int>(-1)) == -1 ) {
      goto end;
    }

  end:
    mBuff.close();
  }
}

// @brief 書き込み可能の時に true を返す．
bool
ZCoder::is_ready() const
{
  return mBuff.is_ready();
}

// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
// @param[in] wbuff 圧縮するデータを格納するバッファ
// @param[in] num 書き込むデータ数(バイト)
// @return 実際に書き込んだバイト数を返す．
// @note エラーが起こったら -1 を返す．
ssize_t
ZCoder::write(const ymuint8* wbuff,
	      ymuint64 num)
{
  if ( num == 0 ) {
    return 0;
  }

  ymuint64 count = num;
  const ymuint8* bp = wbuff;

  if ( m_state == kStart ) {
    m_state = kMiddle;

    m_maxmaxcode = 1UL << m_maxbits;
    ssize_t n = mBuff.write(k_MAGICHEADER, sizeof(k_MAGICHEADER));
    if ( n != sizeof(k_MAGICHEADER) ) {
      return -1;
    }

    ymuint8 tmp = static_cast<ymuint8>(m_maxbits) | m_block_compress;
    n = mBuff.write(&tmp, 1);
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
    for (code_int fcode = m_hsize; fcode < 65536L; fcode *= 2L) {
      ++ m_hshift;
    }
    m_hshift = 8 - m_hshift;

    m_hsize_reg = m_hsize;
    cl_hash(static_cast<count_int>(m_hsize_reg));
  }

  for (code_int i = 0; count -- > 0; ) {
    int disp;
    ymuint8 c = *(bp ++ );
    ++ m_in_count;
    code_int fcode = (static_cast<code_int>(c) << m_maxbits) + m_ent;
    i = (c << m_hshift) ^ m_ent;

    if ( htabof(i) == fcode ) {
      m_ent = codetabof(i);
      continue;
    }
    else if ( static_cast<long>(htabof(i)) < 0 ) {
      goto nomatch;
    }

    if ( i == 0 ) {
      disp = 1;
    }
    else {
      disp = m_hsize_reg - i;
    }

  probe:
    if ( (i -= disp) < 0 ) {
      i += m_hsize_reg;
    }

    if ( htabof(i) == fcode ) {
      m_ent = codetabof(i);
      continue;
    }
    if ( static_cast<long>(htabof(i)) >= 0 ) {
      goto probe;
    }

  nomatch:
    if ( output(m_ent) == -1 ) {
      return -1;
    }

    ++ m_out_count;
    m_ent = c;

    if ( m_free_ent < m_maxmaxcode ) {
      codetabof(i) = m_free_ent ++;
      htabof(i) = fcode;
    }
    else if ( m_in_count >= m_checkpoint && m_block_compress ) {
      if ( cl_block() == -1 ) {
	return -1;
      }
    }
  }

  return num;
}

// Table clear for block compress
int
ZCoder::cl_block()
{
  m_checkpoint = m_in_count + k_CHECK_GAP;

  count_int rat;
  if ( m_in_count > 0x007fffff ) { // Shift will overflow.
    rat = m_bytes_out >> 8;
    if ( rat == 0 ) {
      rat = 0x7fffffff;
    }
    else {
      rat = m_in_count / rat;
    }
  }
  else {
    rat = (m_in_count << 8) / m_bytes_out;
  }
  if ( rat > m_ratio ) {
    m_ratio = rat;
  }
  else {
    m_ratio = 0;
    cl_hash(m_hsize);
    m_free_ent = k_FIRST;
    m_clear_flg = 1;
    if ( output(k_CLEAR) == -1 ) {
      return -1;
    }
  }

  return 0;
}

void
ZCoder::cl_hash(count_int cl_hsize)
{
  long m1 = -1;
  count_int* htab_p = m_htab + cl_hsize;
  long i = cl_hsize - 16;
  do {			/* Might use Sys V memset(3) here. */
    *(htab_p - 16) = m1;
    *(htab_p - 15) = m1;
    *(htab_p - 14) = m1;
    *(htab_p - 13) = m1;
    *(htab_p - 12) = m1;
    *(htab_p - 11) = m1;
    *(htab_p - 10) = m1;
    *(htab_p - 9) = m1;
    *(htab_p - 8) = m1;
    *(htab_p - 7) = m1;
    *(htab_p - 6) = m1;
    *(htab_p - 5) = m1;
    *(htab_p - 4) = m1;
    *(htab_p - 3) = m1;
    *(htab_p - 2) = m1;
    *(htab_p - 1) = m1;
    htab_p -= 16;
  } while ( (i -= 16) >= 0 );
  for (i += 16; i > 0; i--) {
    *(-- htab_p) = m1;
  }
}

int
ZCoder::output(code_int ocode)
{
  int r_off = m_offset;
  ymuint32 bits = m_n_bits;
  char_type* bp = m_buf;

  if ( ocode >= 0 ) {
    // Get to the first byte.
    bp += (r_off >> 3);
    r_off &= 7;

    // Since ocode is always >= 8 bits, only need to mask the first
    // hunk on the left.
    *bp = (*bp & rmask[r_off]) | ((ocode << r_off) & lmask[r_off]);
    bp++;
    bits -= (8 - r_off);
    ocode >>= 8 - r_off;

    // Get any 8 bit parts in the middle (<=1 for up to 16 bits).
    if ( bits >= 8 ) {
      *bp ++ = ocode;
      ocode >>= 8;
      bits -= 8;
    }

    // Last bits.
    if ( bits ) {
      *bp = ocode;
    }
    m_offset += m_n_bits;
    if ( m_offset == (m_n_bits << 3) ) {
      bp = m_buf;
      bits = m_n_bits;
      m_bytes_out += bits;
      ssize_t n = mBuff.write(bp, bits);
      if ( n != bits ) {
	return -1;
      }
      bp += bits;
      bits = 0;
      m_offset = 0;
    }

    // If the next entry is going to be too big for the ocode size,
    // then increase it, if possible.
    if ( m_free_ent > m_maxcode || (m_clear_flg > 0)) {
      // Write the whole buffer, because the input side won't
      // discover the size increase until after it has read it.
      if ( m_offset > 0 ) {
	ssize_t n = mBuff.write(m_buf, m_n_bits);
	if ( n != m_n_bits ) {
	  return -1;
	}
	m_bytes_out += m_n_bits;
      }
      m_offset = 0;

      if ( m_clear_flg ) {
	m_maxcode = MAXCODE(m_n_bits = k_INIT_BITS);
	m_clear_flg = 0;
      }
      else {
	++ m_n_bits;
	if ( m_n_bits == m_maxbits ) {
	  m_maxcode = m_maxmaxcode;
	}
	else {
	  m_maxcode = MAXCODE(m_n_bits);
	}
      }
    }
  }
  else {
    // At EOF, write the rest of the buffer.
    if ( m_offset > 0 ) {
      m_offset = (m_offset + 7) / 8;
      ssize_t n = mBuff.write(m_buf, m_offset);
      if ( n != m_offset ) {
	return -1;
      }
      m_bytes_out += m_offset;
    }
    m_offset = 0;
  }
  return 0;
}

END_NAMESPACE_YM_COMPCOMMON
