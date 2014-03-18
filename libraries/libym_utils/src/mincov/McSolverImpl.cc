
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
  // 検証用にもとの行列をコピーしておく．
  McMatrix orig_matrix(*mMatrix);
  // こちらは McMatrix 自身が持つ復元機能
  mMatrix->save();

  solve_id = 0;

  mBest = UINT_MAX;
  mCurSolution.clear();
  bool stat = solve();
  assert_cond( stat, __FILE__, __LINE__);
  solution = mBestSolution;

  // 復元が正しいかチェックする．
  mMatrix->restore();
  verify_matrix(*mMatrix, orig_matrix);

  // solution がカバーになっているかチェックする．
  assert_cond( mMatrix->verify(solution), __FILE__, __LINE__);

  cout << "Total branch: " << solve_id << endl;

  return mBest;
}

// @brief 解を求める再帰関数
bool
McSolverImpl::solve()
{
  ymuint cur_id = solve_id;
  ++ solve_id;
  if ( mDebug ) {
    ymuint nr = mMatrix->remain_row_size();
    ymuint nc = mMatrix->remain_col_size();
    ymuint32 cur_cost = mMatrix->cost(mCurSolution);
    cout << "solve[#" << cur_id << "](" << nr << " x " << nc << "): "
	 << mBest << " : " << cur_cost << endl;
  }

  mMatrix->reduce(mCurSolution);

  if ( mDebug ) {
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
      if ( mDebug ) {
	cout << "solve[#" << cur_id << "] end: " << mBest << endl
	     << endl;
      }
      return true;
    }
    if ( mDebug ) {
      cout << "solve[#" << cur_id << "] end: bounded" << endl
	   << endl;
    }
    return false;
  }

  ymuint32 lb = mLbCalc(*mMatrix) + tmp_cost;
  if ( lb >= mBest ) {
    if ( mDebug ) {
      cout << "solve[#" << cur_id << "] end: best = " << mBest << ", lb = " << lb << ", bounded" << endl
	   << endl;
    }
    return false;
  }

  vector<McSolverImpl*> solver_list;
  if ( mDoPartition && block_partition(solver_list) ) {
    // ブロック分割を行う．
    if ( mDebug ) {
      cout << "BLOCK PARTITION" << endl;
      matrix().print(cout);
      for (ymuint i = 0; i < solver_list.size(); ++ i) {
	McSolverImpl* solver = solver_list[i];
	cout << "Matrix#" << i << endl;
	solver->matrix().print(cout);
      }
    }
    ymuint32 cost_so_far = mMatrix->cost(mCurSolution);
    ymuint ns = solver_list.size();
    vector<ymuint32> lb_array(ns);
    lb_array[ns - 1] = 0;
    for (ymuint i = 1; i < ns; ++ i) {
      ymuint idx = ns - i - 1;
      McSolverImpl* solver = solver_list[idx + 1];
      ymuint lb = mLbCalc(solver->matrix());
      lb_array[idx] = lb_array[idx + 1] + lb;
    }
    bool bounded = false;
    for (ymuint i = 0; i < ns; ++ i) {
      McSolverImpl* solver = solver_list[i];
      ymuint32 lb_rest = lb_array[i];
      solver->mBest = mBest - cost_so_far - lb_rest;
      solver->mCurSolution.clear();
      bool stat = solver->solve();
      if ( stat ) {
	mCurSolution.insert(mCurSolution.end(), solver->mBestSolution.begin(), solver->mBestSolution.end());
      }
      else {
	bounded = true;
	break;
      }
      cost_so_far += solver->mBest;
    }
    for (ymuint i = 0; i < ns; ++ i) {
      McSolverImpl* solver = solver_list[i];
      delete solver;
    }
    if ( !bounded ) {
      assert_cond( mMatrix->verify(mCurSolution), __FILE__, __LINE__);
      ymuint32 cost = mMatrix->cost(mCurSolution);
      if ( mBest > cost ) {
	mBest = cost;
	mBestSolution = mCurSolution;
	if ( mDebug ) {
	  cout << "solve[#" << cur_id << "] end: " << mBest << endl
	       << endl;
	}
	return true;
      }
    }
    if ( mDebug ) {
      cout << "solve[#" << cur_id << "] end: bounded" << endl
	   << endl;
    }
    return false;
  }

  // 次の分岐のための列をとってくる．
  ymuint col = mSelector(*mMatrix);

#if defined(VERIFY_MINCOV)
  McMatrix orig_matrix(*mMatrix);
  vector<ymuint32> orig_solution(mCurSolution);
#endif

  ymuint cur_n = mCurSolution.size();
  mMatrix->save();

  // その列を選択したときの最良解を求める．
  mMatrix->select_col(col);
  mCurSolution.push_back(col);

  if ( mDebug ) {
    cout << "select column#" << col << endl;
  }

  bool stat1 = solve();

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
    if ( mDebug ) {
      cout << "solve[#" << cur_id << "] end: best = " << mBest << ", lb = " << lb << endl;
    }
    return true;
  }

  // その列を選択しなかったときの最良解を求める．
  mMatrix->delete_col(col);

  if ( mDebug ) {
    cout << "delete column#" << col << endl;
  }

  bool stat2 = solve();

  if ( mDebug ) {
    cout << "solve[#" << cur_id << "] end: " << mBest << endl;
  }

  return stat1 || stat2;
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

struct Gt
{
  bool
  operator()(McSolverImpl* left,
	     McSolverImpl* right)
  {
    if ( left->matrix().remain_col_size() > right->matrix().remain_col_size() ) {
      return true;
    }
    if ( left->matrix().remain_col_size() < right->matrix().remain_col_size() ) {
      return false;
    }
    return left->matrix().remain_row_size() > right->matrix().remain_row_size();
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
  sort(solver_list.begin(), solver_list.end(), Gt());

  return true;
}

// @brief 内部の行列を返す．
const McMatrix&
McSolverImpl::matrix() const
{
  return *mMatrix;
}

// @brief partition フラグを設定する．
void
McSolverImpl::set_partition(bool flag)
{
  mDoPartition = flag;
}

// @brief デバッグフラグを設定する．
void
McSolverImpl::set_debug(bool flag)
{
  mDebug = flag;
}

bool
McSolverImpl::mDoPartition = true;

bool
McSolverImpl::mDebug = false;

END_NAMESPACE_YM_MINCOV
