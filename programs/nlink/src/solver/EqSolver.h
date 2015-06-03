#ifndef EQSOLVER_H
#define EQSOLVER_H

/// @file EqSolver.h
/// @brief EqSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"
#include "YmLogic/VarId.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM_NLINK

class EqNode;
class EqEdge;

//////////////////////////////////////////////////////////////////////
/// @class EqSolver EqSolver.h "EqSolver.h"
/// @brief equality logic を命題論理に変換するソルバ
//////////////////////////////////////////////////////////////////////
class EqSolver
{
public:

  /// @brief コンストラクタ
  /// @param[in] var_num 変数の数
  EqSolver(ymuint var_num);

  /// @brief デストラクタ
  ~EqSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 述語を追加する．
  /// @param[in] solver SATソルバ
  /// @param[in] var1, var2 変数番号
  /// @return 命題変数を返す．
  VarId
  add_equal(SatSolver& solver,
	    ymuint var1,
	    ymuint var2);

  /// @brief 推移的な等価関係を表す制約を加える．
  /// @param[in] solver SATソルバ
  void
  make_constr(SatSolver& solver);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードの配列
  vector<EqNode*> mNodeArray;

  // 枝の配列
  vector<EqEdge*> mEdgeArray;

};

END_NAMESPACE_YM_NLINK

#endif // EQSOLVER_H
