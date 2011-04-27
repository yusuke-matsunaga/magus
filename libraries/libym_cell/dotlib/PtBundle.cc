
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
// @param[in] name 名前
PtBundle::PtBundle(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
PtBundle::~PtBundle()
{
}

// @brief ピンを追加する．
bool
PtBundle::add_pin(PtPin* pin)
{
#warning "TODO: ピン名の重複チェック?"
  mPinList.push_back(pin);
  return true;
}

// @brief 名前を返す．
ShString
PtBundle::name() const
{
  return mName;
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
