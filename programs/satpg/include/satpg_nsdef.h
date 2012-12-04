#ifndef SATPG_NSDEF_H
#define SATPG_NSDEF_H

/// @file satpg_nsdef.h
/// @brief ATPG 用の名前空間の定義
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: atpg_nsdef.h 2128 2009-03-12 07:25:33Z matsunaga $
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.

// 名前空間の定義ファイル
// 必ず先頭でインクルードしなければならない．

// namespace でネストするのがいやなので define マクロでごまかす．

#include "ymtools.h"

#define BEGIN_NAMESPACE_YM_SATPG \
BEGIN_NAMESPACE_YM \
BEGIN_NAMESPACE(nsSatpg)

#define END_NAMESPACE_YM_SATPG \
END_NAMESPACE(nsSatpg) \
END_NAMESPACE_YM


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @brief タイマーのモード
//////////////////////////////////////////////////////////////////////
enum {
  /// @brief その他
  TM_MISC = 0,
  /// @brief ファイル読み込み
  TM_READ = 1,
  /// @brief DTPG
  TM_DTPG = 2,
  /// @brief 故障シミュレーション
  TM_FSIM = 3,
  /// @brief SAT
  TM_SAT  = 4,
  /// @brief タイマーのモード数
  TM_SIZE = 5
};


//////////////////////////////////////////////////////////////////////
/// @brief パタン生成の結果
//////////////////////////////////////////////////////////////////////
enum tStat {
  /// @brief 検出ベクタを生成した．
  kDetect,
  /// @brief 検出不能と判定した．
  kUntest,
  /// @brief アボートした．
  kAbort
};


//////////////////////////////////////////////////////////////////////
/// @brief シミュレーションモード
//////////////////////////////////////////////////////////////////////
enum tSimMode {
  /// @brief なし
  kSimNone,
  /// @brief シングルパタン
  kSimSingle,
  /// @brief パタン並列
  kSimPpsfp
};


END_NAMESPACE_YM_SATPG

#endif // SATPG_NSDEF_H
