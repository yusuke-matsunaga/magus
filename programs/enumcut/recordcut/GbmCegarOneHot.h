#ifndef GBMCEGARONEHOT_H
#define GBMCEGARONEHOT_H

/// @file GbmCegarOneHot.h
/// @brief GbmCegarOneHot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmCegarOneHot GbmCegarOneHot.h "GbmCegarOneHot.h"
/// @brief CEGAR ベースの GbmSolver (2値符号化版)
//////////////////////////////////////////////////////////////////////
class GbmCegarOneHot :
  public GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmCegarOneHot();

  /// @brief デストラクタ
  virtual
  ~GbmCegarOneHot();


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
  ///             iorder[pos] に network の pos 番めの入力に対応した
  ///             関数の入力番号が入る．
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


#endif // GBMCEGARONEHOT_H
