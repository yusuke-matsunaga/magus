#ifndef LIBYM_TECHMAP_CELLMAP_FFGROUP_H
#define LIBYM_TECHMAP_CELLMAP_FFGROUP_H

/// @file libym_techmap/cellmap/FFGroup.h
/// @brief FFGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroup.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class FFGroup FFGroup.h "FFGroup.h"
/// @brief FF のグループを表すセル
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

  // ピン番号をパックしたもの(3bitずつ)
  ymuint32 mPosArray;

};

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_FFGROUP_H
