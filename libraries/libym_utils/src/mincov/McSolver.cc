
/// @file McSolver.cc
/// @brief McSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McSolver.h"
#include "McSolverImpl.h"
#include "McMatrix.h"
#include "LbMIS.h"
#include "LbCS.h"
#include "SelSimple.h"
#include "SelCS.h"


BEGIN_NAMESPACE_YM_MINCOV

bool McSolver_debug = false;

//////////////////////////////////////////////////////////////////////
// クラス McSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
McSolver::McSolver()
{
  mLbCalc = new LbCS();
  //mLbCalc = new LbMIS();
  mSelector = new SelSimple();
  //mSelector = new SelCS();
  mMatrix = NULL;
}

// @brief デストラクタ
McSolver::~McSolver()
{
  delete mLbCalc;
  delete mSelector;
  delete mMatrix;
}

// @brief 問題のサイズを設定する．
// @param[in] row_size 行数
// @param[in] col_size 列数
void
McSolver::set_size(ymuint32 row_size,
		   ymuint32 col_size)
{
  delete mMatrix;
  mMatrix = new McMatrix(row_size, col_size);
}

// @brief 列のコストを設定する
// @param[in] col_pos 追加する要素の列番号
// @param[in] cost コスト
void
McSolver::set_col_cost(ymuint32 col_pos,
		       ymuint32 cost)
{
  mMatrix->set_col_cost(col_pos, cost);
}

// @brief 要素を追加する．
// @param[in] row_pos 追加する要素の行番号
// @param[in] col_pos 追加する要素の列番号
void
McSolver::insert_elem(ymuint32 row_pos,
		      ymuint32 col_pos)
{
  mMatrix->insert_elem(row_pos, col_pos);
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
ymuint32
McSolver::exact(vector<ymuint32>& solution)
{
  McSolverImpl* impl = new McSolverImpl(*mLbCalc, *mSelector);
  impl->set_matrix(*mMatrix);

  ymuint32 cost = impl->exact(solution);

  delete impl;

  return cost;
}

// @brief ヒューリスティックで最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
ymuint32
McSolver::heuristic(vector<ymuint32>& solution)
{
  McMatrix cur_matrix(*mMatrix);

  solution.clear();
  for ( ; ; ) {
    cur_matrix.reduce(solution);

    if ( cur_matrix.remain_row_size() == 0 ) {
      break;
    }

    // 次の分岐のための列をとってくる．
    ymuint col = (*mSelector)(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    solution.push_back(col);

    if ( McSolver_debug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }
  }

  assert_cond( mMatrix->verify(solution), __FILE__, __LINE__);
  ymuint32 cost = mMatrix->cost(solution);

  return cost;
}

// @brief 内部の行列の内容を出力する．
// @param[in] s 出力先のストリーム
void
McSolver::print_matrix(ostream& s)
{
  mMatrix->print(s);
}

END_NAMESPACE_YM_MINCOV
