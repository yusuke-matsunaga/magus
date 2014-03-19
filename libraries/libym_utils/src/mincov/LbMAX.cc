
/// @file LbMAX.cc
/// @brief LbMAX の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LbMAX.h"


BEGIN_NAMESPACE_YM_MINCOV

//////////////////////////////////////////////////////////////////////
// クラス LbMAX
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LbMAX::LbMAX()
{
}

// @brief デストラクタ
LbMAX::~LbMAX()
{
  for (vector<LbCalc*>::iterator p = mCalcList.begin();
       p != mCalcList.end(); ++ p) {
    delete *p;
  }
}

// @brief 下界の計算をする．
ymuint32
LbMAX::operator()(const McMatrix& matrix)
{
  ymuint32 max_val = 0;
  for (vector<LbCalc*>::iterator p = mCalcList.begin();
       p != mCalcList.end(); ++ p) {
    LbCalc& calc = **p;
    ymuint32 val = calc(matrix);
    if ( max_val < val ) {
      max_val = val;
    }
  }
  return max_val;
}

// @brief 下界の計算クラスを追加する．
void
LbMAX::add_calc(LbCalc* calc)
{
  mCalcList.push_back(calc);
}


END_NAMESPACE_YM_MINCOV
