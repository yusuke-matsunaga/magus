
/// @file McSolverImpl.cc
/// @brief McSolverImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McSolverImpl.h"
#include "McMatrix.h"
#include "McRowHead.h"
#include "McColHead.h"
#include "McCell.h"
#include "LbCalc.h"
#include "Selector.h"
#include "utils/MFSet.h"


BEGIN_NAMESPACE_YM_MINCOV

bool mincov_debug = false;

static
ymuint solve_id = 0;

// 2つの行列が等しいかをチェックする関数
// 等しくなければ例外を送出する．
void
verify_matrix(McMatrix& a,
	      McMatrix& b)
{
  assert_cond( a.remain_row_size() == b.remain_row_size(), __FILE__, __LINE__);
  assert_cond( a.remain_col_size() == b.remain_col_size(), __FILE__, __LINE__);
  const McRowHead* row_a = a.row_front();
  const McRowHead* row_b = b.row_front();
  for ( ; ; ) {
    assert_cond( row_a->pos() == row_b->pos(), __FILE__, __LINE__);
    assert_cond( row_a->num() == row_b->num(), __FILE__, __LINE__);
    const McCell* cell_a = row_a->front();
    const McCell* cell_b = row_b->front();
    for ( ; ; ) {
      assert_cond( cell_a->row_pos() == row_a->pos(), __FILE__, __LINE__);
      assert_cond( cell_b->row_pos() == row_b->pos(), __FILE__, __LINE__);
      assert_cond( cell_a->col_pos() == cell_b->col_pos(), __FILE__, __LINE__);
      cell_a = cell_a->row_next();
      cell_b = cell_b->row_next();
      if ( row_a->is_end(cell_a) ) {
	assert_cond( row_b->is_end(cell_b), __FILE__, __LINE__);
	break;
      }
    }
    row_a = row_a->next();
    row_b = row_b->next();
    if ( a.is_row_end(row_a) ) {
      assert_cond( b.is_row_end(row_b), __FILE__, __LINE__);
      break;
    }
  }
  const McColHead* col_a = a.col_front();
  const McColHead* col_b = b.col_front();
  for ( ; ; ) {
    assert_cond( col_a->pos() == col_b->pos(), __FILE__, __LINE__);
    assert_cond( col_a->num() == col_b->num(), __FILE__, __LINE__);
    const McCell* cell_a = col_a->front();
    const McCell* cell_b = col_b->front();
    for ( ; ; ) {
      assert_cond( cell_a->col_pos() == col_a->pos(), __FILE__, __LINE__);
      assert_cond( cell_b->col_pos() == col_b->pos(), __FILE__, __LINE__);
      assert_cond( cell_a->row_pos() == cell_b->row_pos(), __FILE__, __LINE__);
      cell_a = cell_a->col_next();
      cell_b = cell_b->col_next();
      if ( col_a->is_end(cell_a) ) {
	assert_cond( col_b->is_end(cell_b), __FILE__, __LINE__);
	break;
      }
    }
    col_a = col_a->next();
    col_b = col_b->next();
    if ( a.is_col_end(col_a) ) {
      assert_cond( b.is_col_end(col_b), __FILE__, __LINE__);
      break;
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス McSolverImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lb_calc 下界の計算クラス
// @param[in] selector 列を選択するクラス
McSolverImpl::McSolverImpl(LbCalc& lb_calc,
			   Selector& selector) :
  mLbCalc(lb_calc),
  mSelector(selector)
{
  mMatrix = NULL;
}

// @brief デストラクタ
McSolverImpl::~McSolverImpl()
{
  delete mMatrix;
}

// @brief 問題の行列をコピーする．
void
McSolverImpl::set_matrix(const McMatrix& matrix)
{
  delete mMatrix;
  mMatrix = new McMatrix(matrix.row_size(), matrix.col_size());

  for (const McRowHead* row = matrix.row_front();
       !matrix.is_row_end(row); row = row->next()) {
    ymuint row_pos = row->pos();
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint col_pos = cell->col_pos();
      mMatrix->insert_elem(row_pos, col_pos);
    }
  }
  for (ymuint i = 0; i < matrix.col_size(); ++ i) {
    mMatrix->set_col_cost(i, matrix.col_cost(i));
  }
}

// @brief 問題の行列をコピーする．
void
McSolverImpl::set_matrix(const McMatrix& matrix,
			 const vector<const McRowHead*>& row_list)
{
  delete mMatrix;
  mMatrix = new McMatrix(matrix.row_size(), matrix.col_size());

  for (ymuint i = 0; i < row_list.size(); ++ i) {
    const McRowHead* row = row_list[i];
    ymuint row_pos = row->pos();
    for (const McCell* cell = row->front();
	 !row->is_end(cell); cell = cell->row_next()) {
      ymuint col_pos = cell->col_pos();
      mMatrix->insert_elem(row_pos, col_pos);
    }
  }
  for (ymuint i = 0; i < matrix.col_size(); ++ i) {
    mMatrix->set_col_cost(i, matrix.col_cost(i));
  }
}

// @brief 最小被覆問題を解く．
// @param[out] solution 選ばれた列集合
// @return 解のコスト
ymuint32
McSolverImpl::exact(vector<ymuint32>& solution)
{
  McMatrix orig_matrix(*mMatrix);
  mMatrix->save();

  solve_id = 0;

  mBest = UINT_MAX;
  mCurSolution.clear();
  solve();
  solution = mBestSolution;

  mMatrix->restore();
  verify_matrix(*mMatrix, orig_matrix);

  assert_cond( mMatrix->verify(solution), __FILE__, __LINE__);
  return mBest;
}

// @brief 解を求める再帰関数
void
McSolverImpl::solve()
{
  ymuint cur_id = solve_id;
  ++ solve_id;
  if ( mincov_debug ) {
    ymuint nr = mMatrix->remain_row_size();
    ymuint nc = mMatrix->remain_col_size();
    ymuint32 cur_cost = mMatrix->cost(mCurSolution);
    cout << "solve[#" << cur_id << "](" << nr << " x " << nc << "): "
	 << mBest << " : " << cur_cost << endl;
  }

  mMatrix->reduce(mCurSolution);

  if ( mincov_debug ) {
    ymuint nr = mMatrix->remain_row_size();
    ymuint nc = mMatrix->remain_col_size();
    cout << "  after reduce(" << nr << " x " << nc << ")"
	 << ": " << mMatrix->cost(mCurSolution) << endl;
  }

  ymuint32 tmp_cost = mMatrix->cost(mCurSolution);

  if ( mMatrix->remain_row_size() == 0 ) {
    if ( mBest > tmp_cost ) {
      mBest = tmp_cost;
      mBestSolution = mCurSolution;
    }
    if ( mincov_debug ) {
      cout << "solve[#" << cur_id << "] end: " << mBest << endl
	   << endl;
    }
    return;
  }

  ymuint32 lb = mLbCalc(*mMatrix) + tmp_cost;
  if ( lb >= mBest ) {
    if ( mincov_debug ) {
      cout << "solve[#" << cur_id << "] end: bounded" << endl
	   << endl;
    }
    return;
  }

  vector<McSolverImpl*> solver_list;
  if ( false && block_partition(solver_list) ) {
    vector<ymuint32> solution(mCurSolution);
    if ( mincov_debug ) {
      cout << "BLOCK PARTITION" << endl;
      matrix().print(cout);
      for (ymuint i = 0; i < solver_list.size(); ++ i) {
	McSolverImpl* solver = solver_list[i];
	cout << "Matrix#" << i << endl;
	solver->matrix().print(cout);
      }
    }
    for (vector<McSolverImpl*>::iterator p = solver_list.begin();
	 p != solver_list.end(); ++ p) {
      McSolverImpl* solver = *p;
      vector<ymuint32> solution1;
      solver->exact(solution1);
      solution.insert(solution.end(), solution1.begin(), solution1.end());
      delete solver;
    }
    ymuint32 cost = mMatrix->cost(solution);
    if ( mBest > cost ) {
      mBest = cost;
      mBestSolution = solution;
    }
    if ( mincov_debug ) {
      cout << "solve[#" << cur_id << "] end: " << mBest << endl
	   << endl;
    }
    return;
  }

  // 次の分岐のための列をとってくる．
  ymuint col = mSelector(*mMatrix);

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(*mMtrix);
  vector<ymuint32> orig_solution(mCurSolution);
#endif

  ymuint cur_n = mCurSolution.size();
  mMatrix->save();

  // その列を選択したときの最良解を求める．
  mMatrix->select_col(col);
  mCurSolution.push_back(col);

  if ( mincov_debug ) {
    cout << "select column#" << col << endl;
  }

  solve();

  mMatrix->restore();
  ymuint c = mCurSolution.size() - cur_n;
  for (ymuint i = 0; i < c; ++ i) {
    mCurSolution.pop_back();
  }

#if defined(VERIFYY_MINCOV)
  verify_matrix(orig_matrix, *mMatrix);
  assert_cond( orig_solution == mCurSlution, __FILE__, __LINE__);
#endif

  // 今得た最良解が下界と等しかったら探索を続ける必要はない．
  if ( lb >= mBest ) {
    return;
  }

  // その列を選択しなかったときの最良解を求める．
  mMatrix->delete_col(col);

  if ( mincov_debug ) {
    cout << "delete column#" << col << endl;
  }

  solve();

  if ( mincov_debug ) {
    cout << "solve[#" << cur_id << "] end: " << mBest << endl
	 << endl;
  }
}


BEGIN_NONAMESPACE

struct Lt
{
  bool
  operator()(McSolverImpl* left,
	     McSolverImpl* right)
  {
    if ( left->matrix().remain_col_size() < right->matrix().remain_col_size() ) {
      return true;
    }
    if ( left->matrix().remain_col_size() > right->matrix().remain_col_size() ) {
      return false;
    }
    return left->matrix().remain_row_size() < right->matrix().remain_row_size();
  }
};

END_NONAMESPACE

// @brief ブロック分割を行う．
// @param[in] solver_list 分割された小問題のソルバーのリスト
// @retval true ブロック分割が行われた．
// @retval false ブロック分割が行えなかった．
bool
McSolverImpl::block_partition(vector<McSolverImpl*>& solver_list)
{
  ymuint nr = mMatrix->remain_row_size();
  ymuint nc = mMatrix->remain_col_size();

  MFSet mfset(mMatrix->row_size());
  for (const McColHead* col = mMatrix->col_front();
       !mMatrix->is_col_end(col); col = col->next()) {
    vector<ymuint> row_list;
    row_list.reserve(col->num());
    for (const McCell* cell = col->front();
	 !col->is_end(cell); cell = cell->col_next()) {
      row_list.push_back(cell->row_pos());
    }
    ymuint n = row_list.size();
    for (ymuint i1 = 0; i1 < n; ++ i1) {
      ymuint r1 = row_list[i1];
      for (ymuint i2 = i1 + 1; i2 < n; ++ i2) {
	ymuint r2 = row_list[i2];
	mfset.merge(r1, r2);
      }
    }
  }
  vector<ymuint> rep_list;
  for (const McRowHead* row = mMatrix->row_front();
       !mMatrix->is_row_end(row); row = row->next()) {
    ymuint row_pos = row->pos();
    ymuint rep = mfset.find(row_pos);
    if ( rep == row_pos ) {
      rep_list.push_back(rep);
    }
  }
  if ( rep_list.size() == 1 ) {
    return false;
  }

  vector<const McRowHead*> row_list;
  row_list.reserve(mMatrix->remain_row_size());
  for (vector<ymuint>::iterator p = rep_list.begin();
       p != rep_list.end(); ++ p) {
    ymuint rep = *p;
    row_list.clear();
    for (const McRowHead* row = mMatrix->row_front();
	 !mMatrix->is_row_end(row); row = row->next()) {
      ymuint row_pos = row->pos();
      if ( mfset.find(row_pos) == rep ) {
	row_list.push_back(row);
      }
    }
    McSolverImpl* solver = new McSolverImpl(mLbCalc, mSelector);
    solver->set_matrix(*mMatrix, row_list);
    solver_list.push_back(solver);
  }

  // サイズの昇順に整列
  //sort(solver_list.begin(), solver_list.end(), Lt());

  return true;
}

// @brief 内部の行列を返す．
const McMatrix&
McSolverImpl::matrix() const
{
  return *mMatrix;
}

END_NAMESPACE_YM_MINCOV
