
/// @file LdGroup.cc
/// @brief LdGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdGroup.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
// クラス LdGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
LdGroup::LdGroup(ymuint id) :
  mId(id),
  mParent(NULL),
  mCellGroup(NULL)
{
}

// @brief デストラクタ
LdGroup::~LdGroup()
{
}

// @brief ID番号を返す．
ymuint
LdGroup::id() const
{
  return mId;
}

// @brief 親の LdClass を返す．
LdClass*
LdGroup::parent() const
{
  return mParent;
}

// @brief 親のクラスに対する変換マップを返す．
const NpnMapM&
LdGroup::map() const
{
  return mMap;
}

// @brief 実体を返す．
const CellGroup*
LdGroup::cell_group() const
{
  return mCellGroup;
}

// @brief 属しているセルの番号のリストを返す．
const vector<ymuint>&
LdGroup::cell_list() const
{
  return mCellList;
}

// @brief セル番号を追加する．
// @param[in] cell_id セル番号
void
LdGroup::add_cell(ymuint cell_id)
{
  mCellList.push_back(cell_id);
}

END_NAMESPACE_YM_CELL_LIBDUMP
