#ifndef LDLOGICGROUP_H
#define LDLOGICGROUP_H

/// @file LdLogicGroup.h
/// @brief LdLogicGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdCellGroup.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

class LdLogicClass;

//////////////////////////////////////////////////////////////////////
/// @class LdLogicGroup LdLogicGroup.h "LdLogicGroup.h"
/// @brief 論理関数を表すクラス
//////////////////////////////////////////////////////////////////////
class LdLogicGroup :
  public LdCellGroup
{
  friend class LdLogicMgr;

private:

  /// @brief コンストラクタ
  LdLogicGroup();

  /// @brief デストラクタ
  ~LdLogicGroup();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 親の代表クラスを返す．
  const LdLogicClass*
  rep_class() const;

  /// @brief 親に対する変換マップを返す．
  const NpnMap&
  map() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親の代表クラス
  LdLogicClass* mRepClass;

  // 代表関数から自分自身へ変換するマップ
  NpnMap mMap;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdLogicGroup::LdLogicGroup()
{
}

// @brief デストラクタ
inline
LdLogicGroup::~LdLogicGroup()
{
}

// @brief ID番号を返す．
inline
ymuint
LdLogicGroup::id() const
{
  return mId;
}

// @brief 親の代表クラスを返す．
inline
const LdLogicClass*
LdLogicGroup::rep_class() const
{
  return mRepClass;
}

// @brief 親に対する変換マップを返す．
inline
const NpnMap&
LdLogicGroup::map() const
{
  return mMap;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDLOGIC_H
