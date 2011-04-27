
/// @file libym_cell/dotlib/PtLibrary.cc
/// @brief PtLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtLibrary.h"
#include "PtCell.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name ライブラリ名
PtLibrary::PtLibrary(const PtValue* name) :
  mName(name)
{
}

// @brief デストラクタ
PtLibrary::~PtLibrary()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
// @note デフォルトの実装はなにもしない．
void
PtLibrary::add_child(const ShString& attr_name,
		     PtNode* node)
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

// @brief 値の数を返す．
ymuint
PtLibrary::value_num() const
{
  return 1;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtLibrary::value(ymuint pos) const
{
  return mName;
}

END_NAMESPACE_YM_CELL_DOTLIB
