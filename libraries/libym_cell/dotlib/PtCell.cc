
/// @file libym_cell/dotlib/PtCell.cc
/// @brief PtCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtCell.h"
#include "PtPin.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値(ここではセル名)
PtCell::PtCell(const PtValue* value) :
  mValue(value)
{
}

// @brief デストラクタ
PtCell::~PtCell()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
void
PtCell::add_child(const ShString& attr_name,
		  PtNode* node)
{
#if 0
  if ( node->attr_name() == "pin" ) {
    PtPin* pin = dynamic_cast<PtPin*>(node);
    assert_cond( pin != NULL, __FILE__, __LINE__);
    mPinList.push_back(pin);
  }
  else {
    mChildList.push_back(node);
  }
#else
  mChildList.push_back(node);
#endif
}

// @brief 値の数を返す．
// @note このクラスでは常に 1
ymuint
PtCell::value_num() const
{
  return 1;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtCell::value(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mValue;
}

// @brief pin グループの数を返す．
ymuint
PtCell::pin_num() const
{
  return mPinList.size();
}

// @brief pin グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const PtPin*
PtCell::pin(ymuint pos) const
{
  assert_cond( pos < pin_num(), __FILE__, __LINE__);
  return mPinList[pos];
}


END_NAMESPACE_YM_CELL_DOTLIB
