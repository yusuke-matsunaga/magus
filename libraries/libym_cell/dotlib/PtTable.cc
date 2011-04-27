
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
  mName(name)
{
}

// @brief デストラクタ
PtTable::~PtTable()
{
}

// @brief 属性をセットする．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
bool
PtTable::add_complex_attr(const ShString& attr_name,
			  const vector<const PtValue*>& value_list)
{
  if ( attr_name == "index_1" ) {
    mIndex1 = value_list;
  }
  else if ( attr_name == "index_2" ) {
    mIndex2 = value_list;
  }
  else if ( attr_name == "index_3" ) {
    mIndex3 = value_list;
  }
  else if ( attr_name == "values" ) {
    mValues = value_list;
  }
  else {
#warning "TODO: エラーメッセージの出力"
    return false;
  }
  return true;
}

// @brief 値の数を返す．
// @note このクラスでは常に 0
ymuint
PtTable::value_num() const
{
  return 0;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtTable::value(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 名前を返す．
ShString
PtTable::name() const
{
  return mName;
}

END_NAMESPACE_YM_CELL_DOTLIB
