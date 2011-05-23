#ifndef LIBYM_TECHMAP_CELLMAP_LATCHGROUP_H
#define LIBYM_TECHMAP_CELLMAP_LATCHGROUP_H

/// @file libym_techmap/cellmap/LatchGroup.h
/// @brief LatchGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellGroup.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
/// @class LatchGroup LatchGroup.h "LatchGroup.h"
/// @brief ラッチのグループを表すセル
//////////////////////////////////////////////////////////////////////
class LatchGroup :
  public CellGroup
{
  friend class CellMgr;

private:

  /// @brief コンストラクタ
  LatchGroup();

  /// @brief デストラクタ
  ~LatchGroup();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief データ入力のピン番号を返す．
  ymuint
  data_pos() const;

  /// @brief イネーブル入力のピン番号を返す．
  ymuint
  enable_pos() const;

  /// @brief イネーブル入力のタイプを返す．
  /// @retval 0 Low sensitive
  /// @retval 1 High sensitive
  ymuint
  enable_sense() const;

  /// @brief クリア入力のピン番号を返す．
  /// @note クリア入力がない場合の値は不定
  ymuint
  clear_pos() const;

  /// @brief クリア入力のタイプを返す．
  /// @retval 0 Low sensitive
  /// @retval 1 High sensitive
  ymuint
  clear_sense() const;

  /// @brief プリセット入力のピン番号を返す．
  /// @note プリセット入力がない場合の値は不定
  ymuint
  preset_pos() const;

  /// @brief プリセット入力のタイプを返す．
  /// @retval 0 Low sensitive
  /// @retval 1 High sensitive
  ymuint
  preset_sense() const;

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

  // ピン番号などの情報
  ymuint32 mData;

};

END_NAMESPACE_YM_CELLMAP

#endif // LIBYM_TECHMAP_CELLMAP_LATCHGROUP_H
