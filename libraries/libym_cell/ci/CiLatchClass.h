#ifndef CILATCHCLASS_H
#define CILATCHCLASS_H

/// @file CiLatchClass.h
/// @brief CiLatchClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CiClass.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CiLatchClass CiLatchClass.h "CiLatchClass.h"
/// @brief LatchGroup の集合を表すクラス
//////////////////////////////////////////////////////////////////////
class CiLatchClass :
  public CiClass
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CiLatchClass();

  /// @brief デストラクタ
  virtual
  ~CiLatchClass();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力タイプの情報をパックしたもの
  ymuint32 mBits;

};

END_NAMESPACE_YM_CELL

#endif // CILATCHCLASS_H
