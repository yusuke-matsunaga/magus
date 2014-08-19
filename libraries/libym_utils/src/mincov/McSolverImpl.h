#ifndef MCSOLVERIMPL_H
#define MCSOLVERIMPL_H

/// @file McSolverImpl.h
/// @brief McSolverImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mincov_nsdef.h"
#include "McMatrix.h"
#include "LbCalc.h"
#include "Selector.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
/// @class McSolverImpl McSolverImpl.h "McSolverImpl.h"
/// @brief McSolver の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class McSolverImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] matrix 問題の行列
  /// @param[in] lb_calc 下界の計算クラス
  /// @param[in] selector 列を選択するクラス
  McSolverImpl(const McMatrix& matrix,
	       LbCalc& lb_calc,
	       Selector& selector);

  /// @brief コンストラクタ
  /// @param[in] matrix 問題の行列
  /// @param[in] row_list 注目する行番号のリスト
  /// @param[in] col_list 注目する列番号のリスト
  /// @param[in] lb_calc 下界の計算クラス
  /// @param[in] selector 列を選択するクラス
  McSolverImpl(McMatrix& matrix,
	       const vector<ymuint32>& row_list,
	       const vector<ymuint32>& col_list,
	       LbCalc& lb_calc,
	       Selector& selector);

  /// @brief デストラクタ
  ~McSolverImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 最小被覆問題を解く．
  /// @param[out] solution 選ばれた列集合
  /// @return 解のコスト
  ymuint32
  exact(vector<ymuint32>& solution);

  /// @brief 内部の行列を返す．
  const McMatrix&
  matrix() const;

  /// @brief partition フラグを設定する．
  static
  void
  set_partition(bool flag);

  /// @brief デバッグフラグを設定する．
  static
  void
  set_debug(bool flag);

  /// @brief mMaxDepth を設定する．
  static
  void
  set_max_depth(ymuint depth);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 解を求める再帰関数
  bool
  solve(ymuint lb,
	ymuint depth);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 下界の計算クラス
  LbCalc& mLbCalc;

  // 列を選択するクラス
  Selector& mSelector;

  // 問題を表す行列
  McMatrix mMatrix;

  // 現在のベスト
  ymuint32 mBest;

  // 現在のベスト解
  vector<ymuint32> mBestSolution;

  // 現在の解
  vector<ymuint32> mCurSolution;

  // block_partition を行うとき true にするフラグ
  static
  bool mDoPartition;

  // デバッグフラグ
  static
  bool mDebug;

  // デバッグで表示する最大深さ
  static
  ymuint32 mMaxDepth;

};

END_NAMESPACE_YM_MINCOV

#endif // MCSOLVERIMPL_H
