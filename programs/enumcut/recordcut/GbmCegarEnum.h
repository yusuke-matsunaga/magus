#ifndef GBMCEGARENUM_H
#define GBMCEGARENUM_H

/// @file GbmCegarEnum.h
/// @brief GbmCegarEnum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "GbmSolver.h"
#include "GbmLit.h"
#include "RcfNode.h"
#include "ym_logic/SatSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class GbmCegarEnum GbmCegarEnum.h "GbmCegarEnum.h"
/// @brief 単純な GbmSolver
//////////////////////////////////////////////////////////////////////
class GbmCegarEnum :
  public GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmCegarEnum();

  /// @brief デストラクタ
  virtual
  ~GbmCegarEnum();


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

  /// @brief 入力順を考慮したマッチング問題を解く
  /// @param[in] network RcfNetwork
  /// @param[in] output Reconfigurable Network の出力
  /// @param[in] func マッチング対象の関数
  /// @param[in] iorder 入力順序
  /// @param[out] conf_bits configuration ビットの値を収める配列
  /// @note iorder[0] に func の0番めの入力に対応した RcfNetwork の入力番号が入る．
  bool
  _solve_with_order(const RcfNetwork& network,
		    const TvFunc& func,
		    const vector<ymuint>& iorder,
		    vector<bool>& conf_bits);

};

END_NAMESPACE_YM

#endif // GBMCEGARENUM_H
