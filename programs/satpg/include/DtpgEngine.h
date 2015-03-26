﻿#ifndef DTPGENGINE_H
#define DTPGENGINE_H

/// @file DtpgEngine.h
/// @brief DtpgEngine のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "DtpgStats.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgEngine DtpgEngine.h "DtpgEngine.h"
/// @brief DTPG の基本エンジン
//////////////////////////////////////////////////////////////////////
class DtpgEngine
{
public:

  /// @brief デストラクタ
  virtual
  ~DtpgEngine() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief オプション文字列をセットする．
  virtual
  void
  set_option(const string& option_str) = 0;

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

  /// @brief テスト生成を行なう．
  /// @param[in] tpgnetwork 対象のネットワーク
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& tgnetwork,
      DtpgStats& stats) = 0;

};


/// @brief Single エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief Single(k-det) エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
/// @param[in] kdet 多重度
DtpgEngine*
new_DtpgSatKDet(const string& sat_type,
		const string& sat_option,
		ostream* sat_outp,
		BackTracer& bt,
		DetectOp& dop,
		UntestOp& uop,
		ymuint kdet);

/// @brief Single2 エンジンを作る．
/// @param[in] th_val しきい値
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS2(ymuint th_val,
	      const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

/// @brief Single3 エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatS3(const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

/// @brief Multi エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief Multi2 エンジンを作る．
/// @param[in] th_val しきい値
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatM2(ymuint th_val,
	      const string& sat_type,
	      const string& sat_option,
	      ostream* sat_outp,
	      BackTracer& bt,
	      DetectOp& dop,
	      UntestOp& uop);

/// @brief Concurrent エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] max_id ノード番号の最大値 + 1
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_DtpgSatC(const string& sat_type,
	     const string& sat_option,
	     ostream* sat_outp,
	     BackTracer& bt,
	     DetectOp& dop,
	     UntestOp& uop);

/// @brief SmtSingle エンジンを作る．
/// @param[in] sat_type SATソルバの種類を表す文字列
/// @param[in] sat_option SATソルバに渡すオプション文字列
/// @param[in] sat_outp SATソルバ用の出力ストリーム
/// @param[in] bt バックトレーサー
/// @param[in] dop パタンが求められた時に実行されるファンクタ
/// @param[in] uop 検出不能と判定された時に実行されるファンクタ
DtpgEngine*
new_SmtEngineSingle(const string& sat_type,
		    const string& sat_option,
		    ostream* sat_outp,
		    BackTracer& bt,
		    DetectOp& dop,
		    UntestOp& uop);

END_NAMESPACE_YM_SATPG

#endif // DTPGENGINE_H
