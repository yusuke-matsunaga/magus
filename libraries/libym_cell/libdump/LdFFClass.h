#ifndef LDFFCLASS_H
#define LDFFCLASS_H

/// @file LdFFClass.h
/// @brief LdFFClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "libdump_nsdef.h"


BEGIN_NAMESPACE_YM_CELL_LIBDUMP

//////////////////////////////////////////////////////////////////////
/// @class LdFFClass LdFFClass.h "LdFFClass.h"
/// @brief FFのクラスを表すクラス
/// ここでは，ピン位置の違いを無視した同機能のFFの集合をクラスと呼ぶ．
//////////////////////////////////////////////////////////////////////
class LdFFClass
{
  friend class LdFFMgr;

private:

  /// @brief コンストラクタ
  LdFFClass();

  /// @brief デストラクタ
  ~LdFFClass();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief シグネチャを返す．
  ymuint
  signature() const;

  /// @brief クロック入力のタイプを返す．
  ymuint
  clock_sense() const;

  /// @brief クリア入力のタイプを返す．
  ymuint
  clear_sense() const;

  /// @brief プリセット入力のタイプを返す．
  ymuint
  preset_sense() const;

  /// @brief このクラスに属しているFFグループのリストを返す．
  const vector<LdFFGroup*>&
  group_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // ピン情報をパックしたもの
  ymuint32 mBits;

  // FFグループのリスト
  vector<LdFFGroup*> mGroupList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdFFClass::LdFFClass() :
  mId(0),
  mBits(0)
{
}

// @brief デストラクタ
inline
LdFFClass::~LdFFClass()
{
}

// @brief ID番号を返す．
inline
ymuint
LdFFClass::id() const
{
  return mId;
}

// @brief シグネチャを返す．
inline
ymuint
LdFFClass::signature() const
{
  return mBits;
}

// @brief クロック入力のタイプを返す．
inline
ymuint
LdFFClass::clock_sense() const
{
  return (mBits >> 0) & 3U;
}

// @brief クリア入力のタイプを返す．
inline
ymuint
LdFFClass::clear_sense() const
{
  return (mBits >> 2) & 3U;
}

// @brief プリセット入力のタイプを返す．
inline
ymuint
LdFFClass::preset_sense() const
{
  return (mBits >> 4) & 3U;
}

// @brief このクラスに属しているFFグループのリストを返す．
inline
const vector<LdFFGroup*>&
LdFFClass::group_list() const
{
  return mGroupList;
}

END_NAMESPACE_YM_CELL_LIBDUMP

#endif // LDFFCLASS_H
