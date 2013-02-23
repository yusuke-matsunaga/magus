#ifndef DTPG_H
#define DTPG_H

/// @file Dtpg.h
/// @brief Dtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "ym_networks/tgnet.h"
#include "ym_utils/USTime.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgStats Dtpg.h "Dtpg.h"
/// @brief DTPG の統計情報を表すクラス
//////////////////////////////////////////////////////////////////////
struct DtpgStats
{
  /// @brief CNF 式を生成した回数
  ymuint32 mCnfGenCount;

  /// @brief CNF 式の生成に費やした時間
  USTime mCnfGenTime;

  /// @brief テスト生成に成功した回数．
  ymuint32 mDetCount;

  /// @brief テスト生成に成功した時の SAT に要した時間
  USTime mDetTime;

  /// @brief 冗長故障と判定した回数
  ymuint32 mRedCount;

  /// @brief 冗長故障と判定した時の SAT に要した時間
  USTime mRedTime;

  /// @brief アボートした回数
  ymuint32 mAbortCount;

  /// @brief アボートした時の SAT に要した時間
  USTime mAbortTime;

};


//////////////////////////////////////////////////////////////////////
/// @class Dtpg Dtpg.h "Dtpg.h"
/// @brief DTPG を行う基底クラス
//////////////////////////////////////////////////////////////////////
class Dtpg
{
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

  /// @brief get_pat フラグを設定する．
  virtual
  void
  set_get_pat(ymuint val) = 0;

  /// @brief dry-run フラグを設定する．
  virtual
  void
  set_dry_run(bool flag) = 0;

  /// @brief 回路と故障リストを設定する．
  /// @param[in] tgnetwork 対象のネットワーク
  /// @param[in] fault_list 故障リスト
  virtual
  void
  set_network(const TgNetwork& tgnetwork,
	      const vector<SaFault*>& fault_list) = 0;

  /// @brief モードでテスト生成を行なう．
  /// @param[in] op テスト生成後に呼ばれるファンクター
  /// @param[in] option オプション文字列
  virtual
  void
  run(DtpgOperator& op,
      const string& option = string()) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] op テスト生成後に呼ばれるファンクター
  virtual
  void
  run(const vector<SaFault*>& flist,
      DtpgOperator& op) = 0;

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  /// @param[in] po_pos 外部出力番号
  /// @param[in] op テスト生成後に呼ばれるファンクター
  virtual
  void
  run(const vector<SaFault*>& flist,
      ymuint po_pos,
      DtpgOperator& op) = 0;

  /// @brief 統計情報をクリアする．
  virtual
  void
  clear_stats() = 0;

  /// @brief 統計情報を得る．
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  get_stats(DtpgStats& stats) const = 0;

  /// @breif 時間計測を制御する．
  virtual
  void
  timer_enable(bool enable) = 0;

};


/// @brief Dtpg のインスタンスを生成する．
extern
Dtpg*
new_DtpgSat();

END_NAMESPACE_YM_SATPG

#endif // DTPG_H
