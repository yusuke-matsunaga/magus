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

  /// @brief 使用する SAT エンジンを指定する．
  virtual
  void
  set_mode(const string& type = string(),
	   const string& option = string(),
	   ostream* outp = NULL) = 0;

  /// @brief 回路と故障リストを設定する．
  /// @param[in] tpgnetwork 対象のネットワーク
  virtual
  void
  set_network(TpgNetwork& tgnetwork) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] mode メインモード
  /// @param[in] po_mode PO分割モード
  /// @param[in] bt バックトレーサー
  /// @param[in] dop_list DetectOp のリスト
  /// @param[in] uop_list UntestOp のリスト
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(DtpgMode mode,
      tDtpgPoMode po_mode,
      BackTracer& bt,
      const vector<DetectOp*>& dop_list,
      const vector<UntestOp*>& uop_list,
      DtpgStats& stats) = 0;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable) = 0;

};


/// @brief DtpgSat のインスタンスを生成する．
extern
Dtpg*
new_DtpgSat();

END_NAMESPACE_YM_SATPG

#endif // DTPG_H
