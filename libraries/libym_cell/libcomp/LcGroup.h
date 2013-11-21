#ifndef LCGROUP_H
#define LCGROUP_H

/// @file LcGroup.h
/// @brief LcGroup.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libcomp_nsdef.h"
#include "ym_logic/NpnMapM.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcGroup LcGroup.h "LcGroup.h"
/// @brief テンポラリに使用するセルグループ用のクラス
//////////////////////////////////////////////////////////////////////
class LcGroup
{
  friend class LcClass;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  LcGroup(ymuint id);

  /// @brief デストラクタ
  ~LcGroup();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 親の LcClass を返す．
  LcClass*
  parent() const;

  /// @brief 親のクラスに対する変換マップを返す．
  const NpnMapM&
  map() const;

  /// @brief 実体を返す．
  CellGroup*
  cell_group() const;

  /// @brief 属しているセルのリストを返す．
  const vector<Cell*>&
  cell_list() const;

  /// @brief セルを追加する．
  /// @param[in] cell セル
  void
  add_cell(Cell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親の LcClass
  LcClass* mParent;

  // 親のクラスに対する変換マップ
  NpnMapM mMap;

  // 実体のオブジェクト
  CellGroup* mCellGroup;

  // セルのリスト
  vector<Cell*> mCellList;

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCGROUP_H
