
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
PtTable::PtTable(const PtValue* name) :
  mName(name)
{
}

// @brief デストラクタ
PtTable::~PtTable()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
void
PtTable::add_child(const ShString& attr_name,
		    PtNode* node)
{
}

// @brief 値の数を返す．
// @note このクラスでは常に 0
ymuint
PtTable::value_num() const
{
  return 1;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtTable::value(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mName;
}

END_NAMESPACE_YM_CELL_DOTLIB
