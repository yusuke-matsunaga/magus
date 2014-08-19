
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
	       const string& option) :
  YmSat(option),
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
    put_lit(l);
  }
  mOut << endl;

  YmSat::add_clause(lits);
}

// @brief 節を追加する．
// @param[in] lit_num リテラル数
// @param[in] lits リテラルの配列
void
YmSatR::add_clause(ymuint lit_num,
		   const Literal* lits)
{
  mOut << "A";
  for (ymuint i = 0; i < lit_num; ++ i) {
    Literal l = lits[i];
    put_lit(l);
  }
  mOut << endl;

  YmSat::add_clause(lit_num, lits);
}

// @brief 1項の節(リテラル)を追加する．
void
YmSatR::add_clause(Literal lit1)
{
  mOut << "A";
  put_lit(lit1);
  mOut << endl;

  YmSat::add_clause(lit1);
}

// @brief 2項の節を追加する．
void
YmSatR::add_clause(Literal lit1,
		   Literal lit2)
{
  mOut << "A";
  put_lit(lit1);
  put_lit(lit2);
  mOut << endl;

  YmSat::add_clause(lit1, lit2);
}

// @brief 3項の節を追加する．
void
YmSatR::add_clause(Literal lit1,
		   Literal lit2,
		   Literal lit3)
{
  mOut << "A";
  put_lit(lit1);
  put_lit(lit2);
  put_lit(lit3);
  mOut << endl;

  YmSat::add_clause(lit1, lit2, lit3);
}

// @brief 4項の節を追加する．
void
YmSatR::add_clause(Literal lit1,
		   Literal lit2,
		   Literal lit3,
		   Literal lit4)
{
  mOut << "A";
  put_lit(lit1);
  put_lit(lit2);
  put_lit(lit3);
  put_lit(lit4);
  mOut << endl;

  YmSat::add_clause(lit1, lit2, lit3, lit4);
}

// @brief 5項の節を追加する．
void
YmSatR::add_clause(Literal lit1,
		   Literal lit2,
		   Literal lit3,
		   Literal lit4,
		   Literal lit5)
{
  mOut << "A";
  put_lit(lit1);
  put_lit(lit2);
  put_lit(lit3);
  put_lit(lit4);
  put_lit(lit5);
  mOut << endl;

  YmSat::add_clause(lit1, lit2, lit3, lit4, lit5);
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
    put_lit(l);
  }
  mOut << endl;

  Bool3 ans = YmSat::solve(assumptions, model);

  mOut << "# " << ans << endl;

  return ans;
}

// @brief リテラルを出力する．
void
YmSatR::put_lit(Literal lit)
{
  mOut << " " << lit.varid();
  if ( lit.is_positive() ) {
    mOut << "P";
  }
  else {
    mOut << "N";
  }
}

END_NAMESPACE_YM_SAT
