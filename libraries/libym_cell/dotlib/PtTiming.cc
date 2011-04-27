
/// @file libym_cell/dotlib/PtTiming.cc
/// @brief PtTiming の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtTiming.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtTiming
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtTiming::PtTiming()
{
}

// @brief デストラクタ
PtTiming::~PtTiming()
{
}

// @brief テーブルを追加する．
// @param[in] attr_name 属性名
// @param[in] table テーブル
bool
PtTiming::add_table(const ShString& attr_name,
		    PtTable* table)
{
  if ( mTableMap.count(attr_name) > 0 ) {
    // 属性名の重複
#warning "TODO: エラーメッセージ"
    return false;
  }
  mTableMap.insert(make_pair(attr_name, table));
  return true;
}

// @brief simple attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
PtTiming::add_simple_attr(const ShString& attr_name,
			  const PtValue* value)
{
  return PtNode::add_simple_attr(attr_name, value);
}

// @brief complex attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
// @return 設定が失敗したら false を返す．
bool
PtTiming::add_complex_attr(const ShString& attr_name,
			   const vector<const PtValue*>& value_list)
{
  return PtNode::add_complex_attr(attr_name, value_list);
}

END_NAMESPACE_YM_CELL_DOTLIB
