
/// @file SaUIP1.cc
/// @brief SaUIP1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SaUIP1.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_NLINK

// @brief コンストラクタ
// @param[in] solver SATソルバ
SaUIP1::SaUIP1(GraphSatImpl* solver) :
  SaBase(solver)
{
}

// @brief デストラクタ
SaUIP1::~SaUIP1()
{
}

// conflict を解析する．
int
SaUIP1::analyze(SatReason creason,
		vector<Literal>& learnt)
{
  capture(creason, learnt);
  make_minimal(learnt);
  clear_marks();
  return reorder(learnt);
}

// creason の矛盾の原因になっている割り当てのうち，
// - もっとも近い unique identification point
// - 現在のレベルよりも低いレベルの割り当て
// からなるセパレータ集合を learnt に入れる．
void
SaUIP1::capture(SatReason creason,
		vector<Literal>& learnt)
{
  learnt.clear();
  learnt.push_back(Literal()); // place holder

  bool first = true;
  ymuint count = 0;
  ymuint last = last_assign();
  for ( ; ; ) {
    if ( creason.is_clause() ) {
      SatClause* cclause = creason.clause();

      // cclause が学習節なら activity をあげる．
      if ( cclause->is_learnt() ) {
	bump_clause_activity(cclause);
      }

      // cclause 節に含まれるリテラルが以前の decision level
      // で割り当てられていたら学習節に加える．
      // 現在の decision level なら count を増やすだけ．
      // あとで mAssignList をたどれば該当のリテラルは捜し出せる．
      ymuint n = cclause->lit_num();
      // 最初の節は全てのリテラルを対象にするが，
      // 二番目以降の節の最初のリテラルは割り当て結果なので除外する．
      for (ymuint i = 0; i < n; ++ i) {
	Literal q = cclause->lit(i);
	if ( !first && q == cclause->wl0() ) continue;
	VarId var = q.varid();
	int var_level = decision_level(var);
	if ( !get_mark(var) && var_level > 0 ) {
	  set_mark_and_putq(var);
	  bump_var_activity(var);
	  if ( var_level < decision_level() ) {
	    learnt.push_back(q);
	  }
	  else {
	    ++ count;
	  }
	}
      }
    }
    else {
      ASSERT_COND( !first );
      Literal q = creason.literal();
      VarId var = q.varid();
      int var_level = decision_level(var);
      if ( !get_mark(var) && var_level > 0 ) {
	set_mark_and_putq(var);
	bump_var_activity(var);
	if ( var_level < decision_level() ) {
	    learnt.push_back(q);
	}
	else {
	  ++ count;
	}
      }
    }

    first = false;

    // mAssignList に入っている最近の変数で mark の付いたものを探す．
    // つまり conflict clause に含まれていた変数ということ．
    for ( ; ; -- last) {
      Literal q = get_assign(last);
      VarId var = q.varid();
      if ( get_mark(var) ) {
	set_mark(var, false);
	// それを最初のリテラルにする．
	learnt[0] = ~q;
	creason = reason(q.varid());
	-- last;
	-- count;
	break;
      }
#if defined(DEBUG)
      // ここは重いのでコメントアウトしておく
      ASSERT_COND(last > 0 );
#endif
    }
    if ( count == 0 ) {
      // q は first UIP だった．
      break;
    }
  }
}

END_NAMESPACE_YM_NLINK
