#ifndef DTPGSATBASES_H
#define DTPGSATBASES_H

/// @file DtpgSatBaseS.h
/// @brief DtpgSatBaseS のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatBaseS DtpgSatBaseS.h "DtpgSatBaseS.h"
/// @brief 1つの故障を対象とした CNF を生成する DtpgSat
//////////////////////////////////////////////////////////////////////
class DtpgSatBaseS :
  public DtpgSat
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatBaseS(const string& sat_type,
	       const string& sat_option,
	       ostream* sat_outp,
	       BackTracer& bt,
	       DetectOp& dop,
	       UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~DtpgSatBaseS();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] stats 結果を格納する構造体
  virtual
  void
  run(TpgNetwork& network,
      DtpgStats& stats);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] node_set 対象のノード集合
  /// @param[in] f_tgt 対象の故障
  virtual
  void
  run_single(const NodeSet& node_set,
	     TpgFault* f_tgt) = 0;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATBASES_H
