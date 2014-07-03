#ifndef DTPG_H
#define DTPG_H

/// @file Dtpg.h
/// @brief Dtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "DtpgMode.h"
#include "networks/tgnet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Dtpg Dtpg.h "Dtpg.h"
/// @brief DTPG を行う基底クラス
//////////////////////////////////////////////////////////////////////
class Dtpg
{
public:

  /// @brief デストラクタ
  virtual
  ~Dtpg() {}


public:
  //////////////////////////////////////////////////////////////////////
  // パタン生成を行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] option_str オプション文字列
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      DtpgMode mode,
      tDtpgPoMode po_mode,
      SatEngine& sat_engine,
      DtpgStats& stats) = 0;

};


/// @brief DtpgSat のインスタンスを生成する．
extern
Dtpg*
new_DtpgSat();

/// @brief DtpgSat2 のインスタンスを生成する．
extern
Dtpg*
new_DtpgSat2();

/// @brief DtpgSat3 のインスタンスを生成する．
extern
Dtpg*
new_DtpgSat3();

/// @brief DtpgSmt のインスタンスを生成する．
extern
Dtpg*
new_DtpgSmt();

END_NAMESPACE_YM_SATPG

#endif // DTPG_H
