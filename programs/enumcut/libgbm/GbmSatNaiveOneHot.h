#ifndef GBMSATNAIVEONEHOT_H
#define GBMSATNAIVEONEHOT_H

/// @file GbmSatNaiveOneHot.h
/// @brief GbmSatNaiveOneHot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"
#include "GbmLit.h"
#include "RcfNode.h"
#include "YmLogic/SatSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmSatNaive GbmSatNaive.h "GbmSatNaive.h"
/// @brief 単純な GbmSolver (one-hot encoding)
//////////////////////////////////////////////////////////////////////
class GbmSatNaiveOneHot :
  public GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmSatNaiveOneHot();

  /// @brief デストラクタ
  virtual
  ~GbmSatNaiveOneHot();


private:
  //////////////////////////////////////////////////////////////////////
  // GbmSolver の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力順を考慮したマッチング問題を解く
  /// @param[in] network RcfNetwork
  /// @param[in] func マッチング対象の関数
  /// @param[in] rep 関数の対称変数の代表番号を収める配列
  ///            rep[pos] に pos 番めの入力の代表番号が入る．
  /// @param[out] conf_bits configuration ビットの値を収める配列
  /// @param[out] iorder 入力順序
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
  bool
  _solve(const RcfNetwork& network,
	 const TvFunc& func,
	 const vector<ymuint>& rep,
	 vector<bool>& conf_bits,
	 vector<ymuint>& iorder,
	 ymuint& loop_count);

};

END_NAMESPACE_YM

#endif // GBMSATNAIVEONEHOT_H
