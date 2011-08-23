#ifndef YM_CELL_CELLFFCLASS_H
#define YM_CELL_CELLFFCLASS_H

/// @file ym_cell/CellFFClass.h
/// @brief CellFFClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellFFClass CellFFClass.h "ym_cell/CellFFClass.h"
/// @brief CellFFGroup の集合を表すセル
//////////////////////////////////////////////////////////////////////
class CellFFClass
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  CellFFClass();

  /// @brief デストラクタ
  ~CellFFClass();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief シグネチャを返す．
  ymuint
  signature() const;

  /// @brief クロック入力のタイプを返す．
  /// @retval 1 positive edge
  /// @retval 2 negative edge
  ymuint
  clock_sense() const;

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

  /// @brief このクラスに属する CellFFGroup の数を返す．
  ymuint
  group_num() const;

  /// @brief CellFFGroup を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
  const CellFFGroup&
  group(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力タイプの情報をパックしたもの
  ymuint32 mBits;

  // グループ数
  ymuint32 mGroupNum;

  // グループの配列
  const CellFFGroup** mGroupList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
CellFFClass::CellFFClass() :
  mBits(0U)
{
}

// @brief デストラクタ
inline
CellFFClass::~CellFFClass()
{
  // mGroupList は CellMgr が管理している．
}

// @brief シグネチャを返す．
inline
ymuint
CellFFClass::signature() const
{
  return mBits;
}

// @brief クリア入力を持つタイプの時に true を返す．
inline
bool
CellFFClass::has_clear() const
{
  return clear_sense() != 0U;
}

// @brief プリセット入力を持つタイプの時に true を返す．
inline
bool
CellFFClass::has_preset() const
{
  return preset_sense() != 0U;
}

// @brief このクラスに属する CellFFGroup の数を返す．
inline
ymuint
CellFFClass::group_num() const
{
  return mGroupNum;
}

// @brief CellFFGroup を得る．
// @param[in] pos 位置番号 ( 0 <= pos < group_num() )
inline
const CellFFGroup&
CellFFClass::group(ymuint pos) const
{
  return *mGroupList[pos];
}


END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLFFCLASS_H
