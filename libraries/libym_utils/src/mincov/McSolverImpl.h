#ifndef MCSOLVERIMPL_H
#define MCSOLVERIMPL_H

/// @file McSolverImpl.h
/// @brief McSolverImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McSolverImpl McSolverImpl.h "McSolverImpl.h"
/// @brief McSolver の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class McSolverImpl
{
public:

  /// @brief コンストラクタ
  McSolverImpl();

  /// @brief デストラクタ
  ~McSolverImpl();


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
	       double cost);

  /// @brief 要素を追加する．
  /// @param[in] row_pos 追加する要素の行番号
  /// @param[in] col_pos 追加する要素の列番号
  void
  insert_elem(ymuint32 row_pos,
	      ymuint32 col_pos);

  /// @brief 最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @return 解のコスト
  double
  exact(vector<ymuint32>& solution);

  /// @brief ヒューリスティックで最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @return 解のコスト
  double
  heuristic(vector<ymuint32>& solution);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 解を求める再帰関数
  /// @param[in] matrix 対象の行列
  /// @param[in] best_sofar 現時点の最良解
  /// @param[out] solution 選ばれた列集合
  /// @return 解のコスト
  double
  solve(McMatrix& matrix,
	double best_sofar,
	const vector<ymuint32>& cur_solution,
	vector<ymuint32>& solution);

  /// @brief 下限を求める．
  /// @param[in] matrix 対象の行列
  /// @return 下限値
  double
  lower_bound(McMatrix& matrix);

  /// @brief 簡単化を行う．
  /// @param[in] matrix 対象の行列
  /// @param[out] selected_cols 簡単化中で選択された列の集合
  void
  reduce(McMatrix& matrix,
	 vector<ymuint32>& selected_cols);

  /// @brief 行支配を探し，行を削除する．
  /// @param[in] matrix 対象の行列
  /// @return 削除された行があったら true を返す．
  bool
  row_dominance(McMatrix& matrix);

  /// @brief 列支配を探し，列を削除する．
  /// @param[in] matrix 対象の行列
  /// @return 削除された列があったら true を返す．
  bool
  col_dominance(McMatrix& matrix);

  /// @brief 必須列を探し，列を選択する．
  /// @param[in] matrix 対象の行列
  /// @param[out] selected_cols 選択された列を追加する列集合
  /// @return 選択された列があったら true を返す．
  bool
  essential_col(McMatrix& matrix,
		vector<ymuint32>& selected_cols);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 問題を表す行列
  McMatrix* mMatrix;

  // 探索を行わないフラグ
  bool mNoBranch;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

END_NAMESPACE_YM_MINCOV

#endif // MCSOLVERIMPL_H
