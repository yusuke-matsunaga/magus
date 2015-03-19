
/// @file McSolver.cc
/// @brief McSolver の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McSolver.h"
#include "McSolverImpl.h"
#include "McMatrix.h"
#include "LbMAX.h"
#include "LbMIS1.h"
#include "LbMIS2.h"
#include "LbCS.h"
#include "SelSimple.h"
#include "SelCS.h"

#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_MINCOV

bool McSolver_debug = false;

//////////////////////////////////////////////////////////////////////
// クラス McSolver
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
McSolver::McSolver()
{
  LbCalc* calc1 = new LbCS();
  LbCalc* calc2 = new LbMIS1();
  LbMAX* calc3 = new LbMAX();
  calc3->add_calc(calc1);
  calc3->add_calc(calc2);
  mLbCalc = calc3;

  mSelector = new SelSimple();
  //mSelector = new SelCS();
  mMatrix = NULL;
  mCostArray = NULL;
}

// @brief デストラクタ
McSolver::~McSolver()
{
  delete mLbCalc;
  delete mSelector;
  delete mMatrix;
  delete mCostArray;
}

// @brief 問題のサイズを設定する．
// @param[in] row_size 行数
// @param[in] col_size 列数
void
McSolver::set_size(ymuint32 row_size,
		   ymuint32 col_size)
{
  delete mMatrix;
  delete mCostArray;
  mCostArray = new ymuint32[col_size];
  for (ymuint i = 0; i < col_size; ++ i) {
    mCostArray[i] = 1;
  }
  mMatrix = new McMatrix(row_size, col_size, mCostArray);
}

// @brief 列のコストを設定する
// @param[in] col_pos 追加する要素の列番号
// @param[in] cost コスト
void
McSolver::set_col_cost(ymuint32 col_pos,
		       ymuint32 cost)
{
  mCostArray[col_pos] = cost;
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
  McSolverImpl impl(*mMatrix, *mLbCalc, *mSelector);

  ymuint32 cost = impl.exact(solution);

  return cost;
}

// @brief ヒューリスティックで最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @param[in] alg ヒューリスティックの種類
// @return 解のコスト
ymuint32
McSolver::heuristic(vector<ymuint32>& solution,
		    MinCov::AlgType alg)
{
  McMatrix cur_matrix(*mMatrix);

  solution.clear();
  cur_matrix.reduce(solution);

  if ( cur_matrix.row_num() > 0 ) {
    switch ( alg ) {
    case MinCov::kGreedy:
      greedy(cur_matrix, solution);
      break;

    case MinCov::kRandom:
      random(cur_matrix, solution);
      break;

    case MinCov::kMCT:
      break;
    }
  }

  ASSERT_COND( mMatrix->verify(solution) );

  ymuint32 cost = mMatrix->cost(solution);

  return cost;
}

// @grief greedy アルゴリズムで解を求める．
// @param[in] matrix 対象の行列
// @param[out] solution 選ばれた列集合
void
McSolver::greedy(const McMatrix& matrix,
		 vector<ymuint32>& solution)
{
  if ( McSolver_debug ) {
    cout << "McSolver::greedy() start" << endl;
  }

#if 0
  for (ymuint i = 0; i < 10000; ++ i) {
  McMatrix cur_matrix(matrix);
  vector<ymuint32> solution;
  while ( cur_matrix.row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    ymuint col = (*mSelector)(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    solution.push_back(col);

    if ( McSolver_debug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }

    cur_matrix.reduce(solution);
  }
  }
#endif
  McMatrix cur_matrix(matrix);

  while ( cur_matrix.row_num() > 0 ) {
    // 次の分岐のための列をとってくる．
    ymuint col = (*mSelector)(cur_matrix);

    // その列を選択する．
    cur_matrix.select_col(col);
    solution.push_back(col);

    if ( McSolver_debug ) {
      cout << "Col#" << col << " is selected heuristically" << endl;
    }

    cur_matrix.reduce(solution);
  }
}

// @grief naive な random アルゴリズムで解を求める．
// @param[in] matrix 対象の行列
// @param[out] solution 選ばれた列集合
void
McSolver::random(const McMatrix& matrix,
		 vector<ymuint32>& solution)
{
  if ( McSolver_debug ) {
    cout << "McSolver::random() start" << endl;
  }

  RandGen rg;

  ymuint count_limit = 1000;

  bool first = true;
  ymuint32 best_cost = 0;
  vector<ymuint32> best_solution;
  for (ymuint count = 0; count < count_limit; ++ count) {
    McMatrix cur_matrix(matrix);
    vector<ymuint32> cur_solution;

    while ( cur_matrix.row_num() > 0 ) {
      // ランダムに選ぶ
      ymuint col = 0; // 未完
      const McRowHead* rh = cur_matrix.row_front();
      ymuint n = rh->num();
      ASSERT_COND( n > 0 );
      ymuint idx = rg.int32() % n;
      for (const McCell* cell = rh->front();
	   !rh->is_end(cell); cell = cell->row_next(), -- idx) {
	if ( idx == 0 ) {
	  col = cell->col_pos();
	  break;
	}
      }

      // その列を選択する
      cur_matrix.select_col(col);
      cur_solution.push_back(col);

      cur_matrix.reduce(cur_solution);
    }
    ymuint32 cur_cost = matrix.cost(cur_solution);
    if ( first || best_cost > cur_cost ) {
      first = false;
      best_cost = cur_cost;
      best_solution = cur_solution;
      {
	ymuint32 base_cost = matrix.cost(solution);
	cout << "best so far = " << best_cost + base_cost
	     << " ( " << best_cost << " + " << base_cost << " )"<< endl;
      }
    }
  }

  for (vector<ymuint32>::iterator p = best_solution.begin();
       p != best_solution.end(); ++ p) {
    ymuint32 col = *p;
    solution.push_back(col);
  }
}

// @brief 内部の行列の内容を出力する．
// @param[in] s 出力先のストリーム
void
McSolver::print_matrix(ostream& s)
{
  mMatrix->print(s);
}

END_NAMESPACE_YM_MINCOV
