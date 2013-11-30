#ifndef PACKEDVAL_H
#define PACKEDVAL_H

/// @file PackedVal.h
/// @brief 1ワードにパックしたビットベクタ型の定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PackedVal.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

#include "atpg_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
/// @brief 1ワードのビットベクタを表す型
//////////////////////////////////////////////////////////////////////
typedef ymuint64 PackedVal;

/// @brief 全てのビットが0の定数
const PackedVal kPvAll0 = 0UL;

/// @brief 全てのビットが1の定数
const PackedVal kPvAll1 = ~0UL;

/// @brief PackedVal のビット長
const ymuint kPvBitLen = 64;


/// @brief word 中の1のビット数を数える．
/// @param[in] word 対象のワード
/// @return word 中の1のビット数
inline
ymuint
count_ones(PackedVal word)
{
  const ymuint64 mask1  = 0x5555555555555555UL;
  const ymuint64 mask2  = 0x3333333333333333UL;
  const ymuint64 mask4  = 0x0f0f0f0f0f0f0f0fUL;
  const ymuint64 mask8  = 0x00ff00ff00ff00ffUL;
  const ymuint64 mask16 = 0x0000ffff0000ffffUL;
  const ymuint64 mask32 = 0x00000000ffffffffUL;

  word = (word & mask1)  + ((word >>  1) & mask1);
  word = (word & mask2)  + ((word >>  2) & mask2);
  word = (word & mask4)  + ((word >>  4) & mask4);
  word = (word & mask8)  + ((word >>  8) & mask8);
  word = (word & mask16) + ((word >> 16) & mask16);
  word = (word & mask32) + ((word >> 32) & mask32);
  return word;
}

END_NAMESPACE_YM_ATPG

#endif // PACKEDVAL_H
