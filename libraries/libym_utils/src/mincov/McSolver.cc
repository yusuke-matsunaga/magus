
/// @file McSolver.cc
/// @brief McSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "utils/McSolver.h"
#include "McSolverImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス McSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
McSolver::McSolver()
{
  mImpl = new nsMincov::McSolverImpl();
}

// @brief デストラクタ
McSolver::~McSolver()
{
  delete mImpl;
}

// @brief 問題のサイズを設定する．
// @param[in] row_size 行数
// @param[in] col_size 列数
void
McSolver::set_size(ymuint32 row_size,
		   ymuint32 col_size)
{
  mImpl->set_size(row_size, col_size);
}

// @brief 列のコストを設定する
// @param[in] col_pos 追加する要素の列番号
// @param[in] cost コスト
void
McSolver::set_col_cost(ymuint32 col_pos,
		       double cost)
{
  mImpl->set_col_cost(col_pos, cost);
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
McSolver::insert_elem(ymuint32 row_pos,
		      ymuint32 col_pos)
{
  mImpl->insert_elem(row_pos, col_pos);
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
double
McSolver::solve(vector<ymuint32>& solution)
{
  return mImpl->solve(solution);
}

END_NAMESPACE_YM
