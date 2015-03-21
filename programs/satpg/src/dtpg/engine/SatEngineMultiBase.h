#ifndef SATENGINEMULTIBASE_H
#define SATENGINEMULTIBASE_H

/// @file SatEngineMultiBase.h
/// @brief SatEngineMultiBase のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SatEngine.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SatEngineMultiBase SatEngineMultiBase.h "SatEngineMultiBase.h"
/// @brief 複数の故障の検出に使える CNF 式を生成するタイプの SatEngine
//////////////////////////////////////////////////////////////////////
class SatEngineMultiBase :
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
  SatEngineMultiBase(const string& sat_type,
		     const string& sat_option,
		     ostream* sat_outp,
		     const TpgNetwork& network,
		     BackTracer& bt,
		     DetectOp& dop,
		     UntestOp& uop);

  /// @brief デストラクタ
  virtual
  ~SatEngineMultiBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト生成を行なう．
  /// @param[in] f_tgt 対象の故障
  virtual
  void
  run_single(TpgFault* f_tgt);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief D-Chain 制約のCNFを作る．
  void
  make_dchain_cnf(SatSolver& solver,
		  TpgNode* node);

  /// @brief 故障に関係するノードのリストを作る．
  /// @param[in] flist 故障のリスト
  /// @param[out] fnode_list 故障に関係するノードのリスト
  void
  make_fnode_list(const vector<TpgFault*>& flist,
		  vector<TpgNode*>& fnode_list);

  /// @brief fnode の情報をクリアする．
  void
  clear_fnode_list(const vector<TpgNode*>& fnode_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 作業用の配列
  vector<bool> mDone;

};

END_NAMESPACE_YM_SATPG

#endif // SATENGINEMULTI_H
