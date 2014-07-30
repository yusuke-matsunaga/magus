#ifndef COMPRESS_COMMON_H
#define COMPRESS_COMMON_H

/// @file compress_common.h
/// @brief CompressCoder/CompressDecoder に共通の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


/// @brief nsCompCommmon 名前空間の開始
#define BEGIN_NAMESPACE_YM_COMPCOMMON \
  BEGIN_NAMESPACE_YM		      \
  BEGIN_NAMESPACE(nsCompCommon)

/// @brief nsCompCommon 名前空間の終了
#define END_NAMESPACE_YM_COMPCOMMON		\
  END_NAMESPACE(nsCompCommon)			\
  END_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @namespace nsYm::nsCompCommon
/// @brief CompressCoder/CompressDecoder に共通な定義
//////////////////////////////////////////////////////////////////////
BEGIN_NAMESPACE_YM_COMPCOMMON

//////////////////////////////////////////////////////////////////////
// 型の定義
//////////////////////////////////////////////////////////////////////

typedef long code_int;
typedef long count_int;
typedef u_char char_type;


//////////////////////////////////////////////////////////////////////
// 定数
//////////////////////////////////////////////////////////////////////

// Default bits
const ymuint8 k_BITS = 16;

// 95 % occupancy
const ymuint32 k_HSIZE = 69001;

const char_type k_BIT_MASK = 0x1f;

const char_type k_BLOCK_MASK = 0x80;

// Initial number of bits/code
const ymuint32 k_INIT_BITS = 9;

// Ratio check interval
const count_int k_CHECK_GAP = 10000;

// First free entry
const code_int k_FIRST = 257;

// Table clear output code
const code_int k_CLEAR = 256;

// 状態
enum tState {
  kStart,  // 開始
  kMiddle, // 処理中
  kEof     // 末尾
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

inline
ymuint32
MAXCODE(ymuint32 n_bits)
{
  return (1UL << n_bits) - 1;
}

// デバッグ用の表時間数
inline
void
print_code(ostream& s,
	   u_char suffix)
{
  s << static_cast<int>(suffix);
  if ( isprint(suffix) ) {
    s << " \"" << suffix << "\"";
  }
  else if ( suffix == '\t' ) {
    s << " TAB";
  }
  else if ( suffix == '\r' ) {
    s << " CR";
  }
  else if ( suffix == '\n' ) {
    s << " LF";
  }
}

BEGIN_NONAMESPACE

// このファイルをインクルードする度に下のデータの複製が生成される．
// ちょっと無駄だけどまあいいか

// magic number
const char_type k_MAGICHEADER[] = { '\037', '\235' };

// 左端(msb)から1を埋めていくビットパタン
const char_type lmask[] = {
  0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00
};

// 右端(lsb)から1を埋めていくビットパタン
const char_type rmask[] = {
  0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

END_NONAMESPACE

END_NAMESPACE_YM_COMPCOMMON

#endif // COMPRESS_COMMON_H
