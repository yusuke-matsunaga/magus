#ifndef LIBYM_TECHMAP_CELLMAP_LATCHCLASS_H
#define LIBYM_TECHMAP_CELLMAP_LATCHCLASS_H

/// @file libym_techmap/cellmap/LatchClass.h
/// @brief LatchClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP

class LatchGroup;

//////////////////////////////////////////////////////////////////////
/// @class LatchClass LatchClass.h "LatchClass.h"
/// @brief LatchGroup の集合を表すセル
//////////////////////////////////////////////////////////////////////
class LatchClass
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  LatchClass();

  /// @brief デストラクタ
  ~LatchClass();


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
  const LatchGroup&
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
  LatchGroup* mGroupList;

};

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_LATCHCLASS_H
