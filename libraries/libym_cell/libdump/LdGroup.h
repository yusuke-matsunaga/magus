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
  friend class LdClass;

public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  LdGroup(ymuint id);

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

  /// @brief セル番号を追加する．
  /// @param[in] cell_id セル番号
  void
  add_cell(ymuint cell_id);


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

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDGROUP_H
