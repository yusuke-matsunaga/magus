#ifndef LDCLASS_H
#define LDCLASS_H

/// @file LdClass.h
/// @brief LdClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdClass LdClass.h "LdClass.h"
/// @brief テンポラリに使用するセルクラス用のクラス
//////////////////////////////////////////////////////////////////////
class LdClass
{
  friend class LdMgr;

private:

  /// @brief コンストラクタ
  LdClass();

  /// @brief デストラクタ
  ~LdClass();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 実体を返す．
  const CellClass*
  cell_class() const;

  /// @brief このクラスに属しているグループのリストを返す．
  const vector<LdGroup*>&
  group_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 実体のオブジェクト
  const CellClass* mCellClass;

  // グループのリスト
  vector<LdGroup*> mGroupList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdClass::LdClass() :
  mId(0),
  mCellClass(NULL)
{
}

// @brief デストラクタ
inline
LdClass::~LdClass()
{
}


// @brief ID番号を返す．
inline
ymuint
LdClass::id() const
{
  return mId;
}

// @brief 実体を返す．
inline
const CellClass*
LdClass::cell_class() const
{
  return mCellClass;
}

// @brief このクラスに属しているグループのリストを返す．
inline
const vector<LdGroup*>&
LdClass::group_list()
{
  return mGroupList;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDCLASS_H
