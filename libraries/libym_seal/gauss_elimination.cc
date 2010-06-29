
/// @file libym_seal/gauss_elimination.cc
/// @brief gauss_elimination の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gauss_elimination.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "Matrix.h"


BEGIN_NAMESPACE_YM_SEAL

BEGIN_NONAMESPACE

// 単一要素の行に関する簡単化を行う．
ymuint
singleton_elimination(Matrix& matrix,
		      vector<double>& solution,
		      vector<bool>& elim_rows,
		      vector<bool>& elim_cols)
{
  ymuint nv = matrix.row_size();
  ymuint ne = 0;
  for ( ; ; ) {
    ymuint r = 0;
    ymuint c = 0;
    ymuint n = 0;
    for (r = 0; r < nv; ++ r) {
      if ( elim_rows[r] ) {
	continue;
      }
      n = 0;
      for (ymuint j = 0; j < nv; ++ j) {
	if ( matrix.elem(r, j) != 0.0 ) {
	  c = j;
	  ++ n;
	  if ( n > 1 ) {
	    n = 0;
	    break;
	  }
	}
      }
      if ( n == 1 ) {
	break;
      }
    }
    if ( n == 0 ) {
      break;
    }
    assert_cond( n == 1, __FILE__, __LINE__);
    double v = matrix.elem(r, nv) / matrix.elem(r, c);
    solution[c] = v;
    for (ymuint i = 0; i < nv; ++ i) {
      if ( elim_rows[i] ) {
	continue;
      }
      double v1 = matrix.elem(i, c);
      if ( v1 != 0.0 ) {
	matrix.elem(i, c) = 0;
	matrix.elem(i, nv) -= v * v1;
      }
    }
    elim_rows[r] = true;
    elim_cols[c] = true;
    ++ ne;
  }
  return ne;
}

END_NONAMESPACE


// ガウスの消去法を行って連立方程式の解を得る．
bool
gaussian_elimination(const Matrix& src_matrix,
		     vector<double>& solution)
{
  ymuint32 nr = src_matrix.row_size();
  ymuint32 nc = src_matrix.col_size();
  
  if ( nr + 1 != nc ) {
    // 1列は右辺の定数だとして変数と数と方程式の数は等しくなければならない．
    cout << "nr + 1 != nc" << endl;
    return false;
  }
  
  Matrix work(src_matrix);

  vector<double> tmp_solution(nr, 0.0);
  vector<bool> elim_rows(nr, false);
  vector<bool> elim_cols(nr, false);
  ymuint ne = singleton_elimination(work, tmp_solution, elim_rows, elim_cols);
  cout << "singleton_elimination end, ne = " << ne << endl;
  
  ymuint nr2 = nr - ne;
  ymuint nc2 = nc - ne;
  Matrix work2(nr2, nc2);
  vector<ymuint> row_map(nr);
  vector<ymuint> row_rmap(nr2);
  ymuint j = 0;
  for (ymuint i = 0; i < nr; ++ i) {
    if ( !elim_rows[i] ) {
      row_map[j] = i;
      row_rmap[i] = j;
      ++ j;
    }
  }
  assert_cond( j == nr2, __FILE__, __LINE__);
  vector<ymuint> col_map(nr);
  vector<ymuint> col_rmap(nr2);
  j = 0;
  for (ymuint i = 0; i < nr; ++ i) {
    if ( !elim_cols[i] ) {
      col_map[j] = i;
      col_rmap[i] = j;
      ++ j;
    }
  }
  assert_cond( j == nr2, __FILE__, __LINE__);
  for (ymuint i = 0; i < nr2; ++ i) {
    ymuint i2 = row_map[i];
    for (ymuint j = 0; j < nr2; ++ j) {
      ymuint j2 = col_map[j];
      work2.elem(i, j) = work.elem(i2, j2);
    }
    work2.elem(i, nr2) = work.elem(i2, nr);
  }

  vector<ymuint32> row_idx(nr2);
  vector<double> max_elem(nr2);
  for (ymuint32 i = 0; i < nr2; ++ i) {
    row_idx[i] = i;
    double max = 0.0;
    for (ymuint32 j = 0; j < nr2; ++ j) {
      double v = fabs(work2.elem(i, j));
      if ( max < v ) {
	max = v;
      }
    }
    if ( max == 0.0 ) {
      // すべて 0 の行がある．
      cout << "all 0 row" << endl;
      return false;
    }
    max_elem[i] = max;
  }

  // 変数を一つずつ選んでゆく
  for (ymuint32 i = 0; i < nr2; ++ i) {
    // i 番めの係数が最大の行を選ぶ．
    double max = 0.0;
    ymuint32 max_j = 0;
    for (ymuint32 j = i; j < nr2; ++ j) {
      ymuint32 r = row_idx[j];
      double v = work2.elem(r, i);
      if ( v != 0.0 ) {
	v = fabs(v);
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
    ymuint32 r_i = row_idx[i];
    double v_i = work2.elem(r_i, i);
    for (ymuint32 j = i + 1; j < nr2; ++ j) {
      ymuint32 r_j = row_idx[j];
      double v = work2.elem(r_j, i);
      if (  v != 0.0 ) {
	double d = v / v_i;
	// max_elem の更新
	double max = 0.0;
	for (ymuint32 k = i; k < nc; ++ k) {
	  work2.elem(r_j, k) -= work2.elem(r_i, k) * d;
	  double v = fabs(work2.elem(r_j, k));
	  if ( max < v ) {
	    max = v;
	  }
	}
	max_elem[r_j] = max;
      }
    }
  }
  
  // 最後のチェック
  if ( work2.elem(row_idx[nr2 - 1], nr2 - 1) == 0.0 ) {
    cout << "work.elem(row_idx[nr2 - 1], nr2 - 1) == 0.0" << endl;
    return false;
  }

  // 後方代入
  vector<double> solution2(nr2, 0.0);
  for (ymuint32 i = nr2; i -- > 0; ) {
    ymuint32 r = row_idx[i];
    for (ymuint32 j = i + 1; j < nr2; ++ j) {
      work2.elem(r, nr2) -= work2.elem(r, j) * solution2[j];
    }
    solution2[i] = work2.elem(r, nr2) / work2.elem(r, i);
  }

  solution.resize(nr);
  for (ymuint i = 0; i < nr; ++ i) {
    if ( elim_cols[i] ) {
      solution[i] = tmp_solution[i];
    }
    else {
      ymuint i2 = col_rmap[i];
      solution[i] = solution2[i2];
    }
  }
  
#if 0
  // 解の検証
  bool error = false;
  for (ymuint32 i = 0; i < nr; ++ i) {
    double v = 0.0;
    for (ymuint32 j = 0; j < nr; ++ j) {
      v += src_matrix.elem(i, j) * solution[j];
    }
    if ( !is_similar(v, src_matrix.elem(i, nr)) ) {
      cout << "error at " << i << "th row" << endl
	   << " v = " << v << ", c = " << src_matrix.elem(i, nr) << endl;
      double delta = v - src_matrix.elem(i, nr);
      cout << " delta = " << delta << endl;
      error = true;
    }
  }
  assert_cond(!error, __FILE__, __LINE__);
#endif
  
  return true;
}

END_NAMESPACE_YM_SEAL
