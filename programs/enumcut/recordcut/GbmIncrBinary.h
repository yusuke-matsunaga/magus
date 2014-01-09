#ifndef GBMINCRBINARY_H
#define GBMINCRBINARY_H

/// @file GbmIncrBinary.h
/// @brief GbmIncrBinary のヘッダファイル
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
/// @class GbmIncrBinary GbmIncrBinary.h "GbmIncrBinary.h"
/// @brief 単純な GbmSolver
//////////////////////////////////////////////////////////////////////
class GbmIncrBinary :
  public GbmSolver
{
public:

  /// @brief コンストラクタ
  GbmIncrBinary();

  /// @brief デストラクタ
  virtual
  ~GbmIncrBinary();


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

};

END_NAMESPACE_YM

#endif // GBMINCRBINARY_H
