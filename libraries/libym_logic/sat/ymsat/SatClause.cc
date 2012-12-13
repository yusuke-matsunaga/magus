
/// @file libym_logic/sat/ymsat/SatClause.cc
/// @brief SatClause の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: SatClause.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "SatClause.h"
#include "SatReason.h"
#include "Watcher.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// クラス SatClause
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] lit_num リテラル数
// @param[in] lits リテラルの配列
// @param[in] learnt 学習節の場合 true
SatClause::SatClause(ymuint lit_num,
		     Literal* lits,
		     bool learnt)
{
  mSizeLearnt = (lit_num << 1) | static_cast<ymuint>(learnt);
  mActivity = 0.0;
  for (ymuint i = 0; i < lit_num; ++ i) {
    mLits[i] = lits[i];
  }
}

// @brief デストラクタ
SatClause::~SatClause()
{
}

// @brief SatClause の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatClause& c)
{
  ymuint n = c.lit_num();
  if ( n == 2 ) {
    s << "(" << c.lit(0) << " + " << c.lit(1) << ")";
  }
  else {
    // 一旦 vector に入れてソートする．
    vector<Literal> tmp(n);
    for (ymuint i = 0; i < n; ++ i) {
      tmp[i] = c.lit(i);
    }
    sort(tmp.begin() + 1, tmp.end());

    s << "(";
    const char* plus = "";
    for (ymuint i = 0; i < n; ++ i) {
      s << plus << tmp[i];
      plus = " + ";
    }
    s << ")";
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// クラス SatReason
//////////////////////////////////////////////////////////////////////

// @brief SatReason の内容を出力する
ostream&
operator<<(ostream& s,
	   const SatReason& r)
{
  if ( r.is_literal() ) {
    s << r.literal();
  }
  else {
    s << r.clause();
  }
  return s;
}


END_NAMESPACE_YM_SAT
