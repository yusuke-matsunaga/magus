#ifndef SATENGINESINGLEBASE_H
#define SATENGINESINGLEBASE_H

/// @file SatEngineSingleBase.h
/// @brief SatEngineSingleBase のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineSingleBase SatEngineSingleBase.h "SatEngineSingleBase.h"
/// @brief 1つの故障を対象とした CNF を生成する SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineSingleBase :
  public SatEngine
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
  SatEngineSingleBase(const string& sat_type,
		      const string& sat_option,
		      ostream* sat_outp,
		      const TpgNetwork& network,
		      BackTracer& bt,
		      DetectOp& dop,
		      UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~SatEngineSingleBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] flist 対象の故障リスト
  virtual
  void
  run_multi(const vector<TpgFault*>& flist);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief D-Chain 制約のCNFを作る．
  void
  make_dchain_cnf(SatSolver& solver,
		  TpgNode* node,
		  TpgFault* fault);

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINESINGLEBASE_H
