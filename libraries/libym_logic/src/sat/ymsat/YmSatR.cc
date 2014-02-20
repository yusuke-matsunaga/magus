
/// @file YmSatR.cc
/// @brief YmSatR の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmSatR.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
// YmSatR
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmSatR::YmSatR(ostream& out,
	       SatAnalyzer* analyzer,
	       const string& option) :
  YmSat(analyzer, option),
  mOut(out)
{
}

// @brief デストラクタ
YmSatR::~YmSatR()
{
}

// @brief 変数を追加する．
// @return 新しい変数番号を返す．
// @note 変数番号は 0 から始まる．
VarId
YmSatR::new_var()
{
  VarId id = YmSat::new_var();

  mOut << "N" << endl
       << "# varid = " << id << endl;

  return id;
}

// @brief 節を追加する．
// @param[in] lits リテラルのベクタ
void
YmSatR::add_clause(const vector<Literal>& lits)
{
  mOut << "A";
  for (vector<Literal>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    Literal l = *p;
    mOut << " " << l.varid();
    if ( l.pol() == kPolPosi ) {
      mOut << "P";
    }
    else {
      mOut << "N";
    }
  }
  mOut << endl;

  YmSat::add_clause(lits);
}

// @brief SAT 問題を解く．
// @param[in] assumptions あらかじめ仮定する変数の値割り当てリスト
// @param[out] model 充足するときの値の割り当てを格納する配列．
// @retval kB3True 充足した．
// @retval kB3False 充足不能が判明した．
// @retval kB3X わからなかった．
// @note i 番めの変数の割り当て結果は model[i] に入る．
Bool3
YmSatR::solve(const vector<Literal>& assumptions,
		      vector<Bool3>& model)
{
  mOut << "S";
  for (vector<Literal>::const_iterator p = assumptions.begin();
       p != assumptions.end(); ++ p) {
    Literal l = *p;
    mOut << " " << l.varid();
    if ( l.pol() == kPolPosi ) {
      mOut << "P";
    }
    else {
      mOut << "N";
    }
  }
  mOut << endl;

  Bool3 ans = YmSat::solve(assumptions, model);

  mOut << "# " << ans << endl;

  return ans;
}

END_NAMESPACE_YM_SAT
