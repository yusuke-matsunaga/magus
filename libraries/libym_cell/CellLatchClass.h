#ifndef YM_CELL_CELLLATCHCLASS_H
#define YM_CELL_CELLLATCHCLASS_H

/// @file ym_cell/CellLatchClass.h
/// @brief CellLatchClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellLatchClass CellLatchClass.h "ym_cell/CellLatchClass.h"
/// @brief LatchGroup の集合を表すセル
//////////////////////////////////////////////////////////////////////
class CellLatchClass
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellLatchClass();

  /// @brief デストラクタ
  ~CellLatchClass();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief イネーブル入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  ymuint
  enable_sense() const;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  clear_sense() const;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 なし
  /// @retval 1 High sensitive
  /// @retval 2 Low sensitive
  ymuint
  preset_sense() const;

  /// @brief クリア入力を持つタイプの時に true を返す．
  bool
  has_clear() const;

  /// @brief プリセット入力を持つタイプの時に true を返す．
  bool
  has_preset() const;

  /// @brief このクラスに属する LatchGroup の数を返す．
  ymuint
  group_num() const;

  /// @brief LatchGroup を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
  const CellLatchGroup&
  group(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力タイプの情報をパックしたもの
  ymuint32 mBits;

  // グループ数
  ymuint32 mGroupNum;

  // グループの(実体の)配列
  CellLatchGroup* mGroupList;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLLATCHCLASS_H
