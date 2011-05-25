#ifndef LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFGROUP_H
#define LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFGROUP_H

/// @file libym_techmap/cellmap/libdump/LdFFGroup.h
/// @brief LdFFGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LdCellGroup.h"
#include "../FFPosArray.h"


BEGIN_NAMESPACE_YM_CELLMAP_LIBDUMP

class LdFFClass;

//////////////////////////////////////////////////////////////////////
/// @class LdFFGroup LdFFGroup.h "LdFFGroup.h"
/// @brief FFのグループを表すクラス
//////////////////////////////////////////////////////////////////////
class LdFFGroup :
  public LdCellGroup
{
  friend class LdFFMgr;

private:

  /// @brief コンストラクタ
  LdFFGroup();

  /// @brief デストラクタ
  ~LdFFGroup();


public:

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 親の FFClass を返す．
  const LdFFClass*
  parent() const;

  /// @brief シグネチャを返す．
  ymuint
  signature() const;

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief クロック入力のピン番号を返す．
  ymuint
  clock_pos() const;

  /// @brief クリア入力のピン番号を返す．
  ymuint
  clear_pos() const;

  /// @brief プリセット入力のピン番号を返す．
  ymuint
  preset_pos() const;

  /// @brief 肯定出力のピン番号を返す．
  ymuint
  q_pos() const;

  /// @brief 否定出力のピン番号を返す．
  ymuint
  iq_pos() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

  // 親のFFクラス
  LdFFClass* mParent;

  // ピン番号の情報
  FFPosArray mPosArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
LdFFGroup::LdFFGroup()
{
}

// @brief デストラクタ
inline
LdFFGroup::~LdFFGroup()
{
}

// @brief ID番号を返す．
inline
ymuint
LdFFGroup::id() const
{
  return mId;
}

// @brief 親の FFClass を返す．
inline
const LdFFClass*
LdFFGroup::parent() const
{
  return mParent;
}

// @brief シグネチャを返す．
inline
ymuint
LdFFGroup::signature() const
{
  return mPosArray.signature();
}

// @brief データ入力のピン番号を返す．
inline
ymuint
LdFFGroup::data_pos() const
{
  return mPosArray.data_pos();
}

// @brief クロック入力のピン番号を返す．
inline
ymuint
LdFFGroup::clock_pos() const
{
  return mPosArray.clock_pos();
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
inline
ymuint
LdFFGroup::clear_pos() const
{
  return mPosArray.clear_pos();
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
inline
ymuint
LdFFGroup::preset_pos() const
{
  return mPosArray.preset_pos();
}

// @brief 肯定出力のピン番号を返す．
inline
ymuint
LdFFGroup::q_pos() const
{
  return mPosArray.q_pos();
}

// @brief 否定出力のピン番号を返す．
inline
ymuint
LdFFGroup::iq_pos() const
{
  return mPosArray.iq_pos();
}

END_NAMESPACE_YM_CELLMAP_LIBDUMP

#endif // LIBYM_TECHMAP_CELLMAP_LIBDUMP_LDFFGROUP_H
