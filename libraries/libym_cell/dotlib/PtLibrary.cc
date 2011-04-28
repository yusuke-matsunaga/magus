
/// @file libym_cell/dotlib/PtLibrary.cc
/// @brief PtLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtLibrary.h"
#include "PtCell.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

#if 0
//////////////////////////////////////////////////////////////////////
// クラス PtLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name ライブラリ名
PtLibrary::PtLibrary(const ShString& name) :
  mName(name)
{
}

// @brief デストラクタ
PtLibrary::~PtLibrary()
{
}

// @brief セルを追加する．
bool
PtLibrary::add_cell(PtCell* cell)
{
  ShString name = cell->name();
  if ( mCellMap.count(name) > 0 ) {
    // 名前が重複
#warning "TODO: エラーメッセージの出力方法"
    return false;
  }
  mCellList.push_back(cell);
  mCellMap.insert(make_pair(name, cell));
  return true;
}

// @brief simple attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
PtLibrary::add_simple_attr(const ShString& attr_name,
			   const PtValue* value)
{
  return true;
}

// @brief complex attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value_list 値のリスト
// @return 設定が失敗したら false を返す．
bool
PtLibrary::add_complex_attr(const ShString& attr_name,
			    const vector<const PtValue*>& value_list)
{
  return true;
}

// @brief ライブラリ名を得る．
ShString
PtLibrary::name() const
{
  return mName;
}

// @brief セル数を得る．
ymuint
PtLibrary::cell_num() const
{
  return mCellList.size();
}

// @brief セルを得る．
// @param[in] pos 位置番号 ( 0 <= pos < cell_num() )
const PtCell*
PtLibrary::cell(ymuint pos) const
{
  return mCellList[pos];
}

#endif
END_NAMESPACE_YM_CELL_DOTLIB
