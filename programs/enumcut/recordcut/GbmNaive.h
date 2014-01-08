#ifndef GBMNAIVE_H
#define GBMNAIVE_H

/// @file GbmNaive.h
/// @brief GbmNaive のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"
#include "GbmLit.h"
#include "RcfNode.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmNaive GbmNaive.h "GbmNaive.h"
/// @brief 単純な GbmSolver
//////////////////////////////////////////////////////////////////////
class GbmNaive :
  public GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmNaive();

  /// @brief デストラクタ
  virtual
  ~GbmNaive();


private:
  //////////////////////////////////////////////////////////////////////
  // GbmSolver の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力順を考慮したマッチング問題を解く
  /// @param[in] network RcfNetwork
  /// @param[in] output Reconfigurable Network の出力
  /// @param[in] func マッチング対象の関数
  /// @param[out] conf_bits configuration ビットの値を収める配列
  /// @param[out] iorder 入力順序
  /// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
  bool
  _solve(const RcfNetwork& network,
	 const TvFunc& func,
	 vector<bool>& conf_bits,
	 vector<ymuint>& iorder);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの入出力の関係を表す CNF 式を作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] node 対象のノード
  /// @param[in] node_var_array ノードの変数番号の配列
  /// @param[in] conf_var_array 設定変数番号の配列
  bool
  make_node_cnf(SatSolver& solver,
		const RcfNode* node,
		const vector<GbmLit>& node_var_array,
		const vector<GbmLit>& conf_var_array);

  /// @brief ノードの入力を表すリテラルを作る．
  /// @param[in] node 対象のノード
  /// @param[in] node_var_array ノードの変数番号の配列
  /// @param[out] inputs 結果のリテラルを格納する配列
  void
  make_inputs(const RcfNode* node,
	      const vector<GbmLit>& node_var_array,
	      vector<GbmLit>& inputs);

  /// @brief RcfNodeHandle から GbmLit を作る．
  /// @param[in] handle ハンドル
  /// @param[in] node_var_array ノードの変数番号の配列
  GbmLit
  handle_to_lit(RcfNodeHandle handle,
		const vector<GbmLit>& node_var_array);

};

END_NAMESPACE_YM

#endif // GBMNAIVE_H
