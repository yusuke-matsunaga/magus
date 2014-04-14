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
	    ymuint& a4_num,
	    ymuint& b0_num,
	    ymuint& b1_num,
	    ymuint& b2_num,
	    ymuint& b3_num,
	    ymuint& c0_num,
	    ymuint& c1_num,
	    ymuint& c2_num,
	    ymuint& d0_num,
	    ymuint& d1_num,
	    ymuint& d2_num,
	    ymuint& e0_num,
	    ymuint& e1_num,
	    ymuint& e2_num,
	    ymuint& f0_num,
	    ymuint& f1_num,
	    ymuint& g0_num,
	    ymuint& fail_num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct RcfInfo
  {
    // LUT ネットワーク
    RcfNetwork mNetwork;

    // マッチした回数
    ymuint32 mCount;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // A0
  RcfInfo mA0;

  // A1
  RcfInfo mA1;

  // A2
  RcfInfo mA2;

  // A3
  RcfInfo mA3;

  // A$
  RcfInfo mA4;

  // B0
  RcfInfo mB0;

  // B1
  RcfInfo mB1;

  // B2
  RcfInfo mB2;

  // B3
  RcfInfo mB3;

  // C0
  RcfInfo mC0;

  // C1
  RcfInfo mC1;

  // C2
  RcfInfo mC2;

  // D0
  RcfInfo mD0;

  // D1
  RcfInfo mD1;

  // D2
  RcfInfo mD2;

  // E0
  RcfInfo mE0;

  // E1
  RcfInfo mE1;

  // E2
  RcfInfo mE2;

  // F0
  RcfInfo mF0;

  // F1
  RcfInfo mF1;

  // G0
  RcfInfo mG0;

  vector<RcfInfo*> mN6list;

  vector<RcfInfo*> mN7list;

  vector<RcfInfo*> mN8list;

  vector<RcfInfo*> mN9list;

  vector<RcfInfo*> mN10list;

  vector<RcfInfo*> mAllList;

  // 自明なマッチ (<= 5)の回数
  ymuint32 mTrivialNum;

  // マッチしなかった回数
  ymuint32 mFailNum;

};

END_NAMESPACE_YM

#endif // LUT444MATCH_H
