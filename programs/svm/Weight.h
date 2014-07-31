#ifndef WEIGHT_H
#define WEIGHT_H

/// @file Weight.h
/// @brief Weight のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Weight Weight.h "Weight.h"
/// @brief 係数を表すクラス
//////////////////////////////////////////////////////////////////////
struct Weight
{

  /// @brief 空のコンストラクタ
  Weight()
  {
  };

  /// @brief 値を指定したコンストラクタ
  Weight(ymuint idx,
	 double weight) :
    mIdx(idx),
    mWeight(weight)
  {
  }

  // 位置番号
  ymuint32 mIdx;

  // 係数
  double mWeight;

  // 自分より小さいものの係数をすべて足したもの
  double mAcc;

};


//////////////////////////////////////////////////////////////////////
/// @class WeightLT Weight.h "Weight.h"
/// @brief Weight の大小比較用ファンクタ
//////////////////////////////////////////////////////////////////////
struct WeightLT
{

  /// @brief Less-Than 演算
  bool
  operator()(const Weight& left,
	     const Weight& right)
  {
    return left.mWeight < right.mWeight;
  }

};


//////////////////////////////////////////////////////////////////////
/// @class WeightGT Weight.h "Weight.h"
/// @brief Weight の大小比較用ファンクタ
//////////////////////////////////////////////////////////////////////
struct WeightGT
{

  /// @brief Grater-Than 演算
  bool
  operator()(const Weight& left,
	     const Weight& right)
  {
    return left.mWeight > right.mWeight;
  }

};

END_NAMESPACE_YM

#endif // WEIGHT_H
