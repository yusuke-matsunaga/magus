
/// @file libym_techmap/Match.cc
/// @brief Match の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Match.h"
#include "ym_npn/NpnMap.h"


BEGIN_NAMESPACE_YM_TECHMAP

//////////////////////////////////////////////////////////////////////
// クラス Match
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] nl 葉の数(入力数)
Match::Match(ymuint nl) :
  mLeafNum(nl),
  mLeafArray(new const SbjNode*[nl]),
  mInvArray(0U)
{
}

// @brief コピーコンストラクタ
Match::Match(const Match& src) :
  mLeafNum(src.leaf_num()),
  mLeafArray(new const SbjNode*[mLeafNum]),
  mInvArray(src.mInvArray)
{
  for (ymuint i = 0; i < mLeafNum; ++ i) {
    mLeafArray[i] = src.mLeafArray[i];
  }
}

// @brief 代入演算子
const Match&
Match::operator=(const Match& src)
{
  resize(src.leaf_num());
  for (ymuint i = 0; i < mLeafNum; ++ i) {
    mLeafArray[i] = src.mLeafArray[i];
  }
  mInvArray = src.mInvArray;

  return *this;
}

// @brief デストラクタ
inline
Match::~Match()
{
  delete [] mLeafArray;
}

END_NAMESPACE_YM_TECHMAP
