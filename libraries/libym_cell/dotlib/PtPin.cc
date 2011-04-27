
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
// @param[in] name 名前
PtPin::PtPin(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
PtPin::~PtPin()
{
}

// @brief タイミング情報を追加する．
void
PtPin::add_timing(PtTiming* timing)
{
  mTimingList.push_back(timing);
}

// @brief simple attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
PtPin::add_simple_attr(const ShString& attr_name,
		       const PtValue* value)
{
  return PtNode::add_simple_attr(attr_name, value);
}

// @brief complex attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
// @return 設定が失敗したら false を返す．
bool
PtPin::add_complex_attr(const ShString& attr_name,
			const vector<const PtValue*>& value_list)
{
  return PtNode::add_complex_attr(attr_name, value_list);
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

END_NAMESPACE_YM_CELL_DOTLIB
