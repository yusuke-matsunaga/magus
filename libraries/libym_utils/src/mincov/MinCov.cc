
/// @file MinCov.cc
/// @brief MinCov の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmUtils/MinCov.h"
#include "McSolver.h"
#include "McSolverImpl.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MinCov
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MinCov::MinCov()
{
  mSolver = new nsMincov::McSolver();
}

// @brief デストラクタ
MinCov::~MinCov()
{
  delete mSolver;
}

// @brief 問題のサイズを設定する．
// @param[in] row_size 行数
// @param[in] col_size 列数
void
MinCov::set_size(ymuint32 row_size,
		 ymuint32 col_size)
{
  mSolver->set_size(row_size, col_size);
}

// @brief 列のコストを設定する
// @param[in] col_pos 追加する要素の列番号
// @param[in] cost コスト
void
MinCov::set_col_cost(ymuint32 col_pos,
		     ymuint32 cost)
{
  mSolver->set_col_cost(col_pos, cost);
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
MinCov::insert_elem(ymuint32 row_pos,
		    ymuint32 col_pos)
{
  mSolver->insert_elem(row_pos, col_pos);
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
ymuint32
MinCov::exact(vector<ymuint32>& solution)
{
  return mSolver->exact(solution);
}

// @brief ヒューリスティックで最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
ymuint32
MinCov::heuristic(vector<ymuint32>& solution)
{
  return mSolver->heuristic(solution);
}

// @brief partition フラグを設定する．
void
MinCov::set_partition(bool flag)
{
  nsMincov::McSolverImpl::set_partition(flag);
}

// @brief デバッグフラグを設定する．
void
MinCov::set_debug(bool flag)
{
  nsMincov::McSolverImpl::set_debug(flag);
}

// @brief デバッグで表示する最大深さを設定する．
void
MinCov::set_max_depth(ymuint depth)
{
  nsMincov::McSolverImpl::set_max_depth(depth);
}

END_NAMESPACE_YM
