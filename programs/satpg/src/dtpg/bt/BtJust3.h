#ifndef BTJUST3_H
#define BTJUST3_H

/// @file BtJust3.h
/// @brief BtJust3 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "BtJustBase.h"
#include "YmUtils/HashSet.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BtJust3 BtJust3.h "BtJust3.h"
/// @brief 必要なノードのみ正当化する BackTracer
//////////////////////////////////////////////////////////////////////
class BtJust3 :
  public BtJustBase
{
public:

  /// @brief コンストラクタ
  BtJust3();

  /// @brief デストラクタ
  virtual
  ~BtJust3();


public:
  //////////////////////////////////////////////////////////////////////
  // BackTracer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief バックトレースを行なう．
  /// @param[in] fnode 故障のあるノード
  /// @param[in] model SATの値の割り当て結果を収めた配列
  /// @param[in] input_list テストパタンに関係のある入力のリスト
  /// @param[in] output_list 故障伝搬の可能性のある出力のリスト
  /// @param[out] assign_list 値の割当リスト
  virtual
  void
  operator()(TpgNode* fnode,
	     const vector<Bool3>& model,
	     const vector<TpgNode*>& input_list,
	     const vector<TpgNode*>& output_list,
	     NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  bool
  dfs(TpgNode* node,
      const vector<Bool3>& model,
      HashMap<ymuint, bool>& mark,
      vector<TpgNode*>& node_list);

  void
  record_recur(TpgNode* node,
	       const vector<Bool3>& model,
	       HashSet<ymuint>& mark,
	       NodeValList& assign_list);

  void
  record_recur2(TpgNode* node,
		const vector<Bool3>& model,
		HashSet<ymuint>& mark,
		NodeValList& assign_list);

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_SATPG

#endif // BTJUST3_H
