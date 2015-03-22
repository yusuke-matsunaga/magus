#ifndef DTPGSATM_H
#define DTPGSATM_H

/// @file DtpgSatM.h
/// @brief DtpgSatM のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgSatBaseM.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DtpgSatM DtpgSatM.h "DtpgSatM.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class DtpgSatM :
  public DtpgSatBaseM
{
public:

  /// @brief コンストラクタ
  /// @param[in] sat_type SATソルバの種類を表す文字列
  /// @param[in] sat_option SATソルバに渡すオプション文字列
  /// @param[in] sat_outp SATソルバ用の出力ストリーム
  /// @param[in] network 対象のネットワーク
  /// @param[in] bt バックトレーサー
  /// @param[in] dop パタンが求められた時に実行されるファンクタ
  /// @param[in] uop 検出不能と判定された時に実行されるファンクタ
  DtpgSatM(const string& sat_type,
	   const string& sat_option,
	   ostream* sat_outp,
	   const TpgNetwork& network,
	   BackTracer& bt,
	   DetectOp& dop,
	   UntestOp& uop,
	   bool forget);

  /// @brief デストラクタ
  virtual
  ~DtpgSatM();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  virtual
  void
  run_multi(const vector<TpgFault*>& flist);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 作業用のノードリスト
  vector<TpgNode*> mTmpNodeList;

  // forget フラグ
  bool mForget;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGSATM_H
