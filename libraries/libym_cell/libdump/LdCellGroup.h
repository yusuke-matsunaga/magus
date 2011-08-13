#ifndef LDCELLGROUP_H
#define LDCELLGROUP_H

/// @file LdCellGroup.h
/// @brief LdCellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdCellGroup LdCellGroup.h "LdCellGroup.h"
/// @brief 等価なセルの集合を表すクラス
//////////////////////////////////////////////////////////////////////
class LdCellGroup
{
  friend class LdFuncMgr;

protected:

  /// @brief コンストラクタ
  LdCellGroup();

  /// @brief デストラクタ
  ~LdCellGroup();


public:

  /// @brief セル番号を追加する．
  void
  add_cell(ymuint cell_id);

  /// @brief 属しているセルの番号のリストを返す．
  const vector<ymuint>&
  cell_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セル番号のリスト
  vector<ymuint> mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdCellGroup::LdCellGroup()
{
}

// @brief デストラクタ
inline
LdCellGroup::~LdCellGroup()
{
}

// @brief セル番号を追加する．
inline
void
LdCellGroup::add_cell(ymuint cell_id)
{
  mCellList.push_back(cell_id);
}

// @brief 属しているセルの番号のリストを返す．
inline
const vector<ymuint>&
LdCellGroup::cell_list() const
{
  return mCellList;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDCELLGROUP_H
