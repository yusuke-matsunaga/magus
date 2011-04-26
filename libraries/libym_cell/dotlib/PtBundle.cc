
/// @file libym_cell/dotlib/PtBundle.cc
/// @brief PtBundle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtBundle.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtBundle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値(ここではバス名)
PtBundle::PtBundle(const PtValue* value) :
  mValue(value)
{
}

// @brief デストラクタ
PtBundle::~PtBundle()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
void
PtBundle::add_child(const ShString& attr_name,
		 PtNode* node)
{
  mChildList.push_back(node);
}

// @brief ピンを追加する．
void
PtBundle::add_pin(PtPin* pin)
{
  mPinList.push_back(pin);
}

// @brief 値の数を返す．
// @note このクラスでは常に 1
ymuint
PtBundle::value_num() const
{
  return 1;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtBundle::value(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mValue;
}

// @brief pin グループの数を返す．
ymuint
PtBundle::pin_num() const
{
  return mPinList.size();
}

// @brief pin グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const PtPin*
PtBundle::pin(ymuint pos) const
{
  assert_cond( pos < pin_num(), __FILE__, __LINE__);
  return mPinList[pos];
}

END_NAMESPACE_YM_CELL_DOTLIB
