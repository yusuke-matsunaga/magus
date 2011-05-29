#ifndef LIBYM_TECHMAP_CELLMAP_FFGROUP_H
#define LIBYM_TECHMAP_CELLMAP_FFGROUP_H

/// @file libym_techmap/cellmap/FFGroup.h
/// @brief FFGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroup.h"
#include "FFPosArray.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class FFGroup FFGroup.h "FFGroup.h"
/// @brief FF のグループを表すクラス
//////////////////////////////////////////////////////////////////////
class FFGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  FFGroup();

  /// @brief デストラクタ
  ~FFGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief クロック入力のピン番号を返す．
  ymuint
  clock_pos() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  ymuint
  clear_pos() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
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

  // ピン番号の情報
  FFPosArray mPosArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
inline
FFGroup::FFGroup()
{
}

// @brief デストラクタ
inline
FFGroup::~FFGroup()
{
}

// @brief データ入力のピン番号を返す．
inline
ymuint
FFGroup::data_pos() const
{
  return mPosArray.data_pos();
}

// @brief クロック入力のピン番号を返す．
inline
ymuint
FFGroup::clock_pos() const
{
  return mPosArray.clock_pos();
}

// @brief クリア入力のピン番号を返す．
// @note クリア入力がない場合の値は不定
inline
ymuint
FFGroup::clear_pos() const
{
  return mPosArray.clear_pos();
}

// @brief プリセット入力のピン番号を返す．
// @note プリセット入力がない場合の値は不定
inline
ymuint
FFGroup::preset_pos() const
{
  return mPosArray.preset_pos();
}

// @brief 肯定出力のピン番号を返す．
inline
ymuint
FFGroup::q_pos() const
{
  return mPosArray.q_pos();
}

// @brief 否定出力のピン番号を返す．
inline
ymuint
FFGroup::iq_pos() const
{
  return mPosArray.iq_pos();
}

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_FFGROUP_H
