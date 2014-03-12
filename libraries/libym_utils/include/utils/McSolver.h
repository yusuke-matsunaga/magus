#ifndef UTILS_MCSOLVER_H
#define UTILS_MCSOLVER_H

/// @file utils/McSolver.h
/// @brief McSolver のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymtools.h"


BEGIN_NAMESPACE_YM

namespace nsMincov {
  class McSolverImpl;
}

//////////////////////////////////////////////////////////////////////
/// @class McSolver McSolver.h "utils/McSolver.h"
/// @brief 最小被覆問題を解くクラス
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
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実際の処理を行うオブジェクト
  nsMincov::McSolverImpl* mImpl;

};

END_NAMESPACE_YM

#endif // UTILS_MCSOLVER_H
