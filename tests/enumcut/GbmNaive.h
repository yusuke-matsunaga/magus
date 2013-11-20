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
#include "GbmNode.h"
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


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題を解く
  virtual
  bool
  solve(const GbmMgr& mgr,
	GbmNodeHandle output1,
	GbmNodeHandle output2);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードの入出力の関係を表す CNF 式を作る．
  /// @param[in] solver SAT ソルバ
  /// @param[in] node 対象のノード
  /// @param[in] node_var_array ノードの変数番号の配列
  /// @param[in] conf_var_array 設定変数番号の配列
  void
  make_node_cnf(SatSolver& solver,
		const GbmNode* node,
		const vector<GbmLit>& node_var_array,
		const vector<GbmLit>& conf_var_array);

  /// @brief ノードの入力を表すリテラルを作る．
  /// @param[in] node 対象のノード
  /// @param[in] node_var_array ノードの変数番号の配列
  /// @param[out] inputs 結果のリテラルを格納する配列
  void
  make_inputs(const GbmNode* node,
	      const vector<GbmLit>& node_var_array,
	      vector<GbmLit>& inputs);

  /// @brief GbmNodeHandle から GbmLit を作る．
  /// @param[in] handle ハンドル
  /// @param[in] node_var_array ノードの変数番号の配列
  GbmLit
  handle_to_lit(GbmNodeHandle handle,
		const vector<GbmLit>& node_var_array);

};

END_NAMESPACE_YM

#endif // GBMNAIVE_H
