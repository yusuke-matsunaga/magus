
/// @file ZDecoder.cc
/// @brief CpressDecoder の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ZDecoder.h"


BEGIN_NAMESPACE_YM_COMPCOMMON

//////////////////////////////////////////////////////////////////////
// クラス ZDecoder
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
ZDecoder::ZDecoder()
{
  m_state = kStart;

  m_maxbits = k_BITS;	/* User settable max # bits/code. */
  m_maxmaxcode = 1L << m_maxbits;	/* Should NEVER generate this code. */
  m_free_ent = 0;			/* First unused entry. */
  m_block_compress = k_BLOCK_MASK;
  m_clear_flg = 0;

  m_roffset = 0;
  m_size = 0;
}

// @brief デストラクタ
ZDecoder::~ZDecoder()
{
  close();
}

// @brief ファイルを開く
// @param[in] filename ファイル名
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルが存在しない．
//  - ファイルに対する読み出し許可がない．
//  - ファイルの形式が異なる．
bool
ZDecoder::open(const char* filename)
{
  return mBuff.open(filename, O_RDONLY, 0);
}

// @brief ファイルを閉じる．
void
ZDecoder::close()
{
  mBuff.close();
}

// @brief 読み出せるデータがある時に true を返す．
bool
ZDecoder::is_ready() const
{
  return mBuff.is_ready();
}

// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
// @param[in] rbuff 展開したデータを格納するバッファ
// @param[in] num 読み出すデータ数(バイト)
// @return 実際に読み出したバイト数を返す．
// @note エラーが起こったら -1 を返す．
ssize_t
ZDecoder::read(ymuint8* rbuff,
	       ymuint64 num)
{
  if ( num == 0 ) {
    cerr << "num == 0" << endl;
    return 0;
  }

  ymuint64 count = num;
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

  // 先頭のマジックナンバーと最大ビット数を読む．
  {
    ymuint8 header[3];
    ssize_t n = mBuff.read(header, sizeof(header));
    if ( n != sizeof(header) ||
	 memcmp(header, k_MAGICHEADER, sizeof(k_MAGICHEADER)) != 0 ) {
      // ファイルタイプミスマッチ
      cerr << "invalid magic header" << endl;
      return -1;
    }
    m_maxbits = header[2];
    m_block_compress = m_maxbits & k_BLOCK_MASK;
    m_maxbits &= k_BIT_MASK;
    m_maxmaxcode = 1UL << m_maxbits;
    if ( m_maxbits > k_BITS || m_maxbits < 12 ) {
      // EFTYPE
      cerr << "m_maxbits = " << m_maxbits << endl;
      return -1;
    }

    m_n_bits = k_INIT_BITS;
    m_maxcode = MAXCODE(m_n_bits);
  }

  for (code_int code = 255; code >= 0; -- code) {
    tab_prefixof(code) = 0;
    tab_suffixof(code) = static_cast<char_type>(code);
  }
  m_free_ent = m_block_compress ? k_FIRST : 256;

  m_finchar = m_oldcode = getcode();
  if ( m_oldcode == -1 ) {
    return 0;
  }

  *bp ++ = m_finchar;
  -- count;
  init_stack();

  for (code_int code; (code = getcode()) > -1; ) {
    if ( (code == k_CLEAR) && m_block_compress ) {
      for (code = 255; code >= 0; -- code) {
	tab_prefixof(code) = 0;
      }
      m_clear_flg = 1;
      m_free_ent = k_FIRST;
      m_oldcode = -1;
      continue;
    }
    m_incode = code;

    if ( code >= m_free_ent ) {
      if ( code > m_free_ent || m_oldcode == -1 ) {
	// Bad stream
	// EINVAL;
	return -1;
      }
      push_stack(m_finchar);
      code = m_oldcode;
    }

    while ( code >= 256 ) {
      push_stack(tab_suffixof(code));
      code = tab_prefixof(code);
    }
    m_finchar = tab_suffixof(code);
    push_stack(m_finchar);

  middle:
    do {
      if ( count -- == 0 ) {
	return num;
      }
      *bp ++ = pop_stack();
    } while ( !is_empty() );

    {
      code_int code = m_free_ent;
      if ( code < m_maxmaxcode && m_oldcode != -1 ) {
	tab_prefixof(code) = m_oldcode;
	tab_suffixof(code) = m_finchar;
	m_free_ent = code + 1;
      }
    }

    m_oldcode = m_incode;
  }

  m_state = kEof;

 eof:
  return num - count;
}

code_int
ZDecoder::getcode()
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
      m_maxcode = MAXCODE(m_n_bits = k_INIT_BITS);
      m_clear_flg = 0;
    }
    ssize_t n = mBuff.read(m_gbuf, m_n_bits);
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
  m_roffset += m_n_bits;

  return gcode;
}

END_NAMESPACE_YM_COMPCOMMON
