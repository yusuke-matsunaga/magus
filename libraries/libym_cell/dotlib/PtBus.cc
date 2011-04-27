
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
  PtPin(name)
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

// @brief simple attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
PtBus::add_simple_attr(const ShString& attr_name,
		       const PtValue* value)
{
  if ( attr_name == "bus_type" &&
       value->type() == SYMBOL ) {
    mBusType = value->string_value();
    return true;
  }
  return PtPin::add_simple_attr(attr_name, value);
}

// @brief complex attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
// @return 設定が失敗したら false を返す．
bool
PtBus::add_complex_attr(const ShString& attr_name,
			const vector<const PtValue*>& value_list)
{
  return PtPin::add_complex_attr(attr_name, value_list);
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
