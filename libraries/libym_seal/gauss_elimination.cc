
/// @file libym_seal/gauss_elimination.cc
/// @brief gauss_elimination の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gauss_elimination.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Matrix.h"
#include "SMatrix.h"


BEGIN_NAMESPACE_YM_SEAL

// ガウスの消去法を行って連立方程式の解を得る．
bool
gaussian_elimination(const Matrix& src_matrix,
		     vector<double>& solution)
{
  ymuint32 nv = src_matrix.row_size();
  ymuint32 nc = src_matrix.col_size();
  
  if ( nv + 1 != nc ) {
    // 1列は右辺の定数だとして変数と数と方程式の数は等しくなければならない．
    cout << "nr + 1 != nc" << endl;
    return false;
  }

  cout << "initializing SMatrix" << endl;
  SMatrix works(nv);
  vector<ymuint32> row_idx(nv);
  vector<ymuint32> col_idx(nv);
  vector<double> max_elem(nv);
  for (ymuint i = 0; i < nv; ++ i) {
    for (ymuint j = 0; j  < nv; ++ j) {
      double v = src_matrix.elem(i, j);
      if ( v != 0.0 ) {
	works.set_value(i, j, v);
      }
    }
    works.set_const(i, src_matrix.elem(i, nv));
    row_idx[i] = i;
    col_idx[i] = i;
  }

  cout << "singleton elimination" << endl;
  ymuint last = 0;
#if 1
  for ( ; ; ) {
    ymuint r = 0;
    bool found = false;
    for (ymuint i = last; i < nv; ++ i) {
      r = row_idx[i];
      if ( works.row_num(r) == 1 ) {
	found = true;
	break;
      }
    }
    if ( !found ) {
      break;
    }
    SmCell* cell = works.row_top(r);
    double v = works.const_elem(r) / cell->value();
    ymuint c = cell->col_pos();
    for (SmCell* cell = works.col_top(c);
	 cell != works.col_end(c); ) {
      SmCell* tmp = cell;
      cell = tmp->down();
      ymuint r1 = tmp->row_pos();
      if ( r1 != r ) {
	double v1 = tmp->value();
	works.set_value(r1, c, 0.0);
	double cval = works.const_elem(r1);
	works.set_const(r1, cval - v * v1);
      }
    }
    // 行 r と last を入れ替える．
    for (ymuint i = last + 1; i < nv; ++ i) {
      if ( row_idx[i] == r ) {
	row_idx[i] = row_idx[last];
	row_idx[last] = r;
	break;
      }
    }
    // 列 c と last を入れ替える．
    for (ymuint i = last + 1; i < nv; ++ i) {
      if ( col_idx[i] == c ) {
	col_idx[i] = col_idx[last];
	col_idx[last] = c;
	break;
      }
    }
    ++ last;
  }
#if 0
  vector<double> tmp_solution(nv, 0.0);
  vector<bool> mark(nv, false);
  for (ymuint i = 0; i < last; ++ i) {
    ymuint r = row_idx[i];
    ymuint c = col_idx[i];
    SmCell* cell = works.row_top(r);
    assert_cond( cell->row_pos() == r, __FILE__, __LINE__);
    assert_cond( cell->col_pos() == c, __FILE__, __LINE__);
    assert_cond( works.row_num(r) == 1, __FILE__, __LINE__);
    assert_cond( works.col_num(c) == 1, __FILE__, __LINE__);
    double v = works.const_elem(r
  }
  for (ymuint i = 0; i < last; ++ i) {
    ymuint r = row_idx[i];
    
  }
#endif
#endif
  cout << "end, last = " << last << endl;
  // 以後は last 〜 (nv - 1) の間だけみればよい．

  // max_elem の計算
  for (ymuint i = last; i < nv; ++ i) {
    ymuint r = row_idx[i];
    if ( works.row_num(r) == 0 ) {
      // すべて 0 の行がある．
      cout << "all 0 row" << endl;
      return false;
    }
    double max = 0.0;
    for (SmCell* cell = works.row_top(r);
	 cell != works.row_end(r); cell = cell->right()) {
      double v = fabs(cell->value());
      if ( max < v ) {
	max = v;
      }
    }
    max_elem[r] = max;
  }

  // 変数を一つずつ選んでゆく
  for (ymuint i = last; i < nv; ++ i) {
    ymuint c = col_idx[i];
    // c 番めの係数が最大の行を選ぶ．
    double max = 0.0;
    ymuint max_j = 0;
    for (ymuint j = i; j < nv; ++ j) {
      ymuint r = row_idx[j];
      SmCell* cell = works.find_elem(r, c);
      if ( cell ) {
	double v = fabs(cell->value());
	v /= max_elem[r];
	if ( max < v ) {
	  max = v;
	  max_j = j;
	}
      }
    }
    if ( max == 0.0 ) {
      // 全ての行でこの変数が消えてしまった．
      cout << "all 0 col" << endl;
      return false;
    }

    // i 番めと max_j 番めの行を入れ替える．
    if ( i != max_j ) {
      ymuint32 tmp = row_idx[i];
      row_idx[i] = row_idx[max_j];
      row_idx[max_j] = tmp;
    }

    // i 番め以降の行からこの変数を消去する．
    // 具体的には i番目以降の行 (j行)から i行 に A_ji / A_ii をかけた
    // ものを引く．
    // ただし， A_ji = 0 の時はなにもしない．
    ymuint32 r_i = row_idx[i];
    for (ymuint32 j = i + 1; j < nv; ++ j) {
      ymuint32 r_j = row_idx[j];
      SmCell* cell = works.find_elem(r_j, c);
      if (  cell ) {
	works.pivot(r_i, c, r_j);
	
	// max_elem の更新
	double max = 0.0;
	for (SmCell* cell = works.row_top(r_j);
	     cell != works.row_end(r_j); cell = cell->right()) {
	  double v = fabs(cell->value());
	  if ( max < v ) {
	    max = v;
	  }
	}
	max_elem[r_j] = max;
      }
    }
  }

#if 0
  // 最後のチェック
  if ( work2.elem(row_idx[nr2 - 1], nr2 - 1) == 0.0 ) {
    cout << "work.elem(row_idx[nr2 - 1], nr2 - 1) == 0.0" << endl;
    return false;
  }
#endif
  
  // 後方代入
  solution.clear();
  solution.resize(nv);
  for (ymuint i = nv; i -- > 0; ) {
    ymuint r = row_idx[i];
    ymuint c = col_idx[i];
    double v = works.const_elem(r);
    double k = 0;
    for (SmCell* cell = works.row_top(r);
	 cell != works.row_end(r); cell = cell->right()) {
      if ( cell->col_pos() == c ) {
	k = cell->value();
      }
      else {
	v -= cell->value() * solution[cell->col_pos()];
      }
    }
    solution[c] = v / k;
  }
  
#if 0
  // 解の検証
  bool error = false;
  for (ymuint i = 0; i < nv; ++ i) {
    double v = 0.0;
    for (ymuint j = 0; j < nv; ++ j) {
      v += src_matrix.elem(i, j) * solution[j];
    }
    double c = src_matrix.elem(i, nv);
    double delta = v - src_matrix.elem(i, nv);
    if ( fabs(delta) >= 1e-10 ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << c << endl
	   << " delta = " << delta << endl;
      error = true;
    }
#if 0
    if ( !is_similar(v, src_matrix.elem(i, nv)) ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << src_matrix.elem(i, nv) << endl;
      double delta = v - src_matrix.elem(i, nv);
      cout << " delta = " << delta << endl;
      error = true;
    }
#endif
  }
  assert_cond(!error, __FILE__, __LINE__);
#endif
  
  return true;
}

END_NAMESPACE_YM_SEAL
