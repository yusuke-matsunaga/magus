
/// @file libym_cell/dotlib/PtTable.cc
/// @brief PtTable の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtTable.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtTable
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name テンプレート名
PtTable::PtTable(const ShString& name) :
  mName(name),
  mDomain(NULL)
{
}

// @brief デストラクタ
PtTable::~PtTable()
{
}

// @brief 名前を返す．
ShString
PtTable::name() const
{
  return mName;
}

/// @brief index_1 属性の値を返す．
const vector<const PtValue*>&
PtTable::index_1() const
{
  return mIndex1;
}

// @brief index_2 属性の値を返す．
const vector<const PtValue*>&
PtTable::index_2() const
{
  return mIndex2;
}

// @brief index_3 属性の値を返す．
const vector<const PtValue*>&
PtTable::index_3() const
{
  return mIndex3;
}

// @brief values 属性の値を返す．
const vector<const PtValue*>&
PtTable::values() const
{
  return mValues;
}

// @brief domain グループを返す．
const PtDomain*
PtTable::domain() const
{
  return mDomain;
}

END_NAMESPACE_YM_CELL_DOTLIB
