
/// @file libym_cell/dotlib/PtPin.cc
/// @brief PtPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtPin.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] attr_name 属性名(ここでは pin)
// @param[in] attr_loc 属性名のファイル上の位置
// @param[in] value 値(ここではピン名)
PtPin::PtPin(const string& attr_name,
	     const FileRegion& attr_loc,
	     const PtValue* value) :
  PtNode(attr_name, attr_loc),
  mValue(value)
{
}

// @brief デストラクタ
PtPin::~PtPin()
{
}

// @brief 子供を追加する．
// @param[in] node 追加する子供のノード
void
PtPin::add_child(PtNode* node)
{
  if ( node->attr_name() == "timing" ) {
    mTimingList.push_back(node);
  }
  else {
    mChildList.push_back(node);
  }
}

// @brief 値の数を返す．
// @note このクラスでは常に 1
ymuint
PtPin::value_num() const
{
  return 1;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtPin::value(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mValue;
}

// @brief 子供のノードの要素数を返す．
ymuint
PtPin::child_num() const
{
  return mChildList.size();
}

// @brief 子供のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
const PtNode*
PtPin::child(ymuint pos) const
{
  assert_cond( pos < child_num(), __FILE__, __LINE__);
  return mChildList[pos];
}

// @brief timing グループの数を返す．
ymuint
PtPin::timing_num() const
{
  return mTimingList.size();
}

// @brief timing グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
const PtNode*
PtPin::timing(ymuint pos) const
{
  assert_cond( pos < timing_num(), __FILE__, __LINE__);
  return mTimingList[pos];
}

END_NAMESPACE_YM_CELL_DOTLIB
