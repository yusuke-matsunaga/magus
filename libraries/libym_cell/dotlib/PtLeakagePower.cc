
/// @file libym_cell/dotlib/PtLeakagePower.h
/// @brief PtLeakagePower の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtLeakagePower.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtLeakagePower
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtLeakagePower::PtLeakagePower()
{
}

// @brief デストラクタ
PtLeakagePower::~PtLeakagePower()
{
}

// @brief 値の数を返す．
// @note simple attribute なら常に1
ymuint
PtLeakagePower::value_num() const
{
  return 0;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtLeakagePower::value(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_CELL_DOTLIB
