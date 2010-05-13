
/// @file magus/src/seal/gauss_elimination.cc
/// @brief gauss_elimination の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: gauss_elimination.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include "magus_config.h"
#endif

#include "Matrix.h"


BEGIN_NAMESPACE_MAGUS_SEAL

// ガウスの消去法を行って連立方程式の解を得る．
bool
gaussian_elimination(const Matrix& src_matrix,
		     vector<double>& solution)
{
  ymuint32 nr = src_matrix.row_size();
  ymuint32 nc = src_matrix.col_size();
  
  if ( nr + 1 != nc ) {
    // 1列は右辺の定数だとして変数と数と方程式の数は等しくなければならない．
    return false;
  }
  
  Matrix work(src_matrix);
  
  vector<ymuint32> row_idx(nr);
  vector<double> max_elem(nr);
  for (ymuint32 i = 0; i < nr; ++ i) {
    row_idx[i] = i;
    double max = 0.0;
    for (ymuint32 j = 0; j < nr; ++ j) {
      double v = fabs(work.elem(i, j));
      if ( max < v ) {
	max = v;
      }
    }
    if ( max == 0.0 ) {
      // すべて 0 の行がある．
      return false;
    }
    max_elem[i] = max;
  }

  // 変数を一つずつ選んでゆく
  for (ymuint32 i = 0; i < nr; ++ i) {
    // i 番めの係数が最大の行を選ぶ．
    double max = 0.0;
    ymuint32 max_j = 0;
    for (ymuint32 j = i; j < nr; ++ j) {
      ymuint32 r = row_idx[j];
      double v = fabs(work.elem(r, i));
      v /= max_elem[r];
      if ( max < v ) {
	max = v;
	max_j = j;
      }
    }
    if ( max == 0.0 ) {
      // 全ての行でこの変数が消えてしまった．
      return false;
    }

    // i 番めと max_j 番めの行を入れ替える．
    if ( i != max_j ) {
      ymuint32 tmp = row_idx[i];
      row_idx[i] = row_idx[max_j];
      row_idx[max_j] = tmp;
    }

    // i 番め以降の行からこの変数を消去する．
    for (ymuint32 j = i + 1; j < nr; ++ j) {
      ymuint32 r_i = row_idx[i];
      ymuint32 r_j = row_idx[j];
      double d = work.elem(r_j, i) / work.elem(r_i, i);
      // max_elem の更新
      double max = 0.0;
      for (ymuint32 k = i; k < nc; ++ k) {
	work.elem(r_j, k) -= work.elem(r_i, k) * d;
	double v = fabs(work.elem(r_j, k));
	if ( max < v ) {
	  max = v;
	}
      }
      max_elem[r_j] = max;
    }
  }
  
  // 最後のチェック
  if ( work.elem(row_idx[nr - 1], nr - 1) == 0.0 ) {
    return false;
  }

  // 後方代入
  solution.clear();
  solution.resize(nr);
  for (ymuint32 i = nr; i -- > 0; ) {
    ymuint32 r = row_idx[i];
    for (ymuint32 j = i; j < nr - 1; ++ j) {
      work.elem(r, nr) -= work.elem(r, j + 1) * solution[j + 1];
    }
    solution[i] = work.elem(r, nr) / work.elem(r, i);
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

END_NAMESPACE_MAGUS_SEAL
