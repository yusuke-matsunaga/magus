
/// @file libym_cell/dotlib/PtPin.cc
/// @brief PtPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtPin.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

#if 0
//////////////////////////////////////////////////////////////////////
// クラス PtPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
PtPin::PtPin(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
PtPin::~PtPin()
{
}

// @brief 名前を返す．
ShString
PtPin::name() const
{
  return mName;
}

// @brief timing グループの数を返す．
ymuint
PtPin::timing_num() const
{
  return mTimingList.size();
}

// @brief timing グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const PtTiming*
PtPin::timing(ymuint pos) const
{
  assert_cond( pos < timing_num(), __FILE__, __LINE__);
  return mTimingList[pos];
}

#endif

END_NAMESPACE_YM_CELL_DOTLIB
