
/// @file libym_cell/dotlib/PtBus.cc
/// @brief PtBus の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtBus.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtBus
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
PtBus::PtBus(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
PtBus::~PtBus()
{
}

// @brief ピンを追加する．
void
PtBus::add_pin(PtPin* pin)
{
  mPinList.push_back(pin);
}

// @brief 値の数を返す．
// @note このクラスでは常に 0
ymuint
PtBus::value_num() const
{
  return 0;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtBus::value(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 名前を返す．
ShString
PtBus::name() const
{
  return mName;
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
