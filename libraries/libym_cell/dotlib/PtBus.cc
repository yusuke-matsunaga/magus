
/// @file libym_cell/dotlib/PtBus.cc
/// @brief PtBus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtBus.h"

#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtBus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
PtBus::PtBus(const ShString& name) :
  PtPin(name)
{
}

// @brief デストラクタ
PtBus::~PtBus()
{
}

// @brief bus_type 属性の値を返す．
ShString
PtBus::bus_type() const
{
  return mBusType;
}

// @brief pin グループの数を返す．
ymuint
PtBus::pin_num() const
{
  return mPinList.size();
}

// @brief pin グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const PtPin*
PtBus::pin(ymuint pos) const
{
  assert_cond( pos < pin_num(), __FILE__, __LINE__);
  return mPinList[pos];
}

END_NAMESPACE_YM_CELL_DOTLIB
