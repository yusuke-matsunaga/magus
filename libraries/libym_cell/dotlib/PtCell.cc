
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
// @param[in] name セル名
PtCell::PtCell(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
PtCell::~PtCell()
{
}

// @brief leakge_power を追加する．
// @param[in] lp 対象の leakage_power
bool
PtCell::add_leakage_power(PtLeakagePower* lp)
{
  mLeakagePowerList.push_back(lp);
  return true;
}

// @brief ピンを追加する．
bool
PtCell::add_pin(PtPin* pin)
{
  mPinList.push_back(pin);
  return true;
}

// @brief バスを追加する．
// @param[in] bus 対象のバス
bool
PtCell::add_bus(PtBus* bus)
{
  mBusList.push_back(bus);
  return true;
}

// @brief バンドルを追加する．
// @param[in] bundle 対象のバンドル
bool
PtCell::add_bundle(PtBundle* bundle)
{
  mBundleList.push_back(bundle);
  return true;
}

// @brief simple attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
PtCell::add_simple_attr(const ShString& attr_name,
			const PtValue* value)
{
  return true;
}

// @brief complex attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
// @return 設定が失敗したら false を返す．
bool
PtCell::add_complex_attr(const ShString& attr_name,
			 const vector<const PtValue*>& value_list)
{
  return true;
}

// @brief セル名を返す．
ShString
PtCell::name() const
{
  return mName;
}

// @brief leakage_power グループの数を返す．
ymuint
PtCell::leakage_power_num() const
{
  return mLeakagePowerList.size();
}

// @brief leakage_power グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < leakage_power_num() )
const PtLeakagePower*
PtCell::leakage_power(ymuint pos) const
{
  assert_cond( pos < leakage_power_num(), __FILE__, __LINE__);
  return mLeakagePowerList[pos];
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

// @brief bus グループの数を返す．
ymuint
PtCell::bus_num() const
{
  return mBusList.size();
}

// @brief bus グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < bus_num() )
const PtBus*
PtCell::bus(ymuint pos) const
{
  assert_cond( pos < bus_num(), __FILE__, __LINE__);
  return mBusList[pos];
}

// @brief bundle グループの数を返す．
ymuint
PtCell::bundle_num() const
{
  return mBundleList.size();
}

// @brief bundle グループのパース木を返す．
// @param[in] pos 位置番号 ( 0 <= pos < bundle_num() )
const PtBundle*
PtCell::bundle(ymuint pos) const
{
  assert_cond( pos < bundle_num(), __FILE__, __LINE__);
  return mBundleList[pos];
}

END_NAMESPACE_YM_CELL_DOTLIB
