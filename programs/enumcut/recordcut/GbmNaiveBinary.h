#ifndef GBMNAIVEBINARY_H
#define GBMNAIVEBINARY_H

/// @file GbmNaiveBinary.h
/// @brief GbmNaiveBinary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"
#include "GbmLit.h"
#include "RcfNode.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmNaiveBinary GbmNaiveBinary.h "GbmNaiveBinary.h"
/// @brief 単純な GbmSolver
//////////////////////////////////////////////////////////////////////
class GbmNaiveBinary :
  public GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmNaiveBinary();

  /// @brief デストラクタ
  virtual
  ~GbmNaiveBinary();


private:
  //////////////////////////////////////////////////////////////////////
  // GbmSolver の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力順を考慮したマッチング問題を解く
  /// @param[in] network RcfNetwork
  /// @param[in] output Reconfigurable Network の出力
  /// @param[in] func マッチング対象の関数
  /// @param[out] conf_bits configuration ビットの値を収める配列
  /// @param[out] iorder 入力順序
  /// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
  bool
  _solve(const RcfNetwork& network,
	 const TvFunc& func,
	 vector<bool>& conf_bits,
	 vector<ymuint>& iorder);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM

#endif // GBMNAIVEBINARY_H
