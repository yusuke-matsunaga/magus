#ifndef MCSOLVER_H
#define MCSOLVER_H

/// @file McSolver.h
/// @brief McSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"
#include "YmAlgo/MinCov.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McSolver McSolver.h "McSolver.h"
/// @brief Minimum Covering 問題を解くクラス
//////////////////////////////////////////////////////////////////////
class McSolver
{
public:

  /// @brief コンストラクタ
  McSolver();

  /// @brief デストラクタ
  ~McSolver();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 問題のサイズを設定する．
  /// @param[in] row_size 行数
  /// @param[in] col_size 列数
  void
  set_size(ymuint32 row_size,
	   ymuint32 col_size);

  /// @brief 列のコストを設定する
  /// @param[in] col_pos 追加する要素の列番号
  /// @param[in] cost コスト
  void
  set_col_cost(ymuint32 col_pos,
	       ymuint32 cost);

  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  void
  insert_elem(ymuint32 row_pos,
	      ymuint32 col_pos);

  /// @brief 最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @return 解のコスト
  ymuint32
  exact(vector<ymuint32>& solution);

  /// @brief ヒューリスティックで最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @param[in] alg ヒューリスティックの種類
  /// @return 解のコスト
  ymuint32
  heuristic(vector<ymuint32>& solution,
	    MinCov::AlgType alg);

  /// @brief 内部の行列の内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print_matrix(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @grief greedy アルゴリズムで解を求める．
  /// @param[in] matrix 対象の行列
  /// @param[out] solution 選ばれた列集合
  void
  greedy(const McMatrix& matrix,
	 vector<ymuint32>& solution);

  /// @grief naive な random アルゴリズムで解を求める．
  /// @param[in] matrix 対象の行列
  /// @param[out] solution 選ばれた列集合
  void
  random(const McMatrix& matrix,
	 vector<ymuint32>& solution);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下界を計算するクラス
  LbCalc* mLbCalc;

  // 列を選ぶクラス
  Selector* mSelector;

  // 問題を表す行列
  McMatrix* mMatrix;

  // コストの配列
  ymuint32* mCostArray;

};

END_NAMESPACE_YM_MINCOV

#endif // MCSOLVER_H
