#ifndef LDGROUP_H
#define LDGROUP_H

/// @file LdGroup.h
/// @brief LdGroup.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdGroup LdGroup.h "LdGroup.h"
/// @brief テンポラリに使用するセルグループ用のクラス
//////////////////////////////////////////////////////////////////////
class LdGroup
{
  friend class LdMgr;

private:

  /// @brief コンストラクタ
  LdGroup();

  /// @brief デストラクタ
  ~LdGroup();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 親の LdClass を返す．
  LdClass*
  parent() const;

  /// @brief 実体を返す．
  const CellGroup*
  cell_group() const;

  /// @brief 属しているセルの番号のリストを返す．
  const vector<ymuint>&
  cell_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親の LdClass
  LdClass* mParent;

  // 実体のオブジェクト
  CellGroup* mCellGroup;

  // セル番号のリスト
  vector<ymuint> mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdGroup::LdGroup() :
  mId(0),
  mParent(NULL),
  mCellGroup(NULL)
{
}

// @brief デストラクタ
inline
LdGroup::~LdGroup()
{
}

// @brief ID番号を返す．
inline
ymuint
LdGroup::id() const
{
  return mId;
}

// @brief 親の LdClass を返す．
inline
LdClass*
LdGroup::parent() const
{
  return mParent;
}

// @brief 実体を返す．
inline
const CellGroup*
LdGroup::cell_group() const
{
  return mCellGroup;
}

// @brief 属しているセルの番号のリストを返す．
inline
const vector<ymuint>&
LdGroup::cell_list() const
{
  return mCellList;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDGROUP_H
