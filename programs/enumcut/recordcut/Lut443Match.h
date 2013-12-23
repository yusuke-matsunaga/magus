#ifndef LUT443MATCH_H
#define LUT443MATCH_H

/// @file Lut443Match.h
/// @brief Lut443Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNetwork.h"
#include "ym_logic/TvFunc.h"
#include "GbmSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Lut443Match Lut443Match.h "Lut443Match.h"
/// @brief 4-4-3 の LUT ネットワークに対するマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class Lut443Match
{
public:

  /// @brief コンストラクタ
  Lut443Match();

  /// @brief デストラクタ
  ~Lut443Match();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
  /// @param[in] func 対象の関数
  /// @param[in] solver GBM ソルバ
  bool
  match(const TvFunc& func,
	GbmSolver& solver);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // A0
  RcfNetwork mA0;

  // A1
  RcfNetwork mA1;

  // A2
  RcfNetwork mA2;

  // A3
  RcfNetwork mA3;

  // B0
  RcfNetwork mB0;

  // B1
  RcfNetwork mB1;

  // B2
  RcfNetwork mB2;

  // C0
  RcfNetwork mC0;

  // C1
  RcfNetwork mC1;

};

END_NAMESPACE_YM

#endif // LUT443MATCH_H
