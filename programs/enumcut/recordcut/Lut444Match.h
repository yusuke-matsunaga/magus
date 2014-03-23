#ifndef LUT444MATCH_H
#define LUT444MATCH_H

/// @file Lut444Match.h
/// @brief Lut444Match のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNetwork.h"
#include "logic/TvFunc.h"
#include "GbmSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Lut444Match Lut444Match.h "Lut444Match.h"
/// @brief 4-4-4 の LUT ネットワークに対するマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class Lut444Match
{
public:

  /// @brief コンストラクタ
  Lut444Match();

  /// @brief デストラクタ
  ~Lut444Match();


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

  /// @brief カウンタをリセットする．
  void
  reset_count();

  /// @brief カウンタの値を得る．
  void
  get_count(ymuint& trivial_num,
	    ymuint& a0_num,
	    ymuint& a1_num,
	    ymuint& a2_num,
	    ymuint& a3_num,
	    ymuint& b0_num,
	    ymuint& b1_num,
	    ymuint& b2_num,
	    ymuint& c0_num,
	    ymuint& c1_num,
	    ymuint& fail_num);


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

  // 自明なマッチ (<= 5)の回数
  ymuint32 mTrivialNum;

  // A0 の回数
  ymuint32 mA0Num;

  // A1 の回数
  ymuint32 mA1Num;

  // A2 の回数
  ymuint32 mA2Num;

  // A3 の回数
  ymuint32 mA3Num;

  // B0 の回数
  ymuint32 mB0Num;

  // B1 の回数
  ymuint32 mB1Num;

  // B2 の回数
  ymuint32 mB2Num;

  // C0 の回数
  ymuint32 mC0Num;

  // C1 の回数
  ymuint32 mC1Num;

  // マッチしなかった回数
  ymuint32 mFailNum;

};

END_NAMESPACE_YM

#endif // LUT444MATCH_H
