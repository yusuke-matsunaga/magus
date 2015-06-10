
/// @file SaUIP1.cc
/// @brief SaUIP1 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SaUIP1.h"
#include "SatClause.h"

#define DEBUG_CAPTURE 0


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
#if DEBUG_CAPTURE
  cout << endl
       << "CAPTURE" << endl
       << creason << endl;
#endif

  learnt.clear();
  learnt.push_back(Literal()); // place holder

  bool first = true;
  ymuint count = 0;
  ymuint last = last_assign();
  for (SatReason tmp_reason = creason; ; ) {
#if DEBUG_CAPTURE
    cout << " tmp_reason = " << tmp_reason << endl;
#endif

    if ( tmp_reason.is_clause() ) {
      SatClause* cclause = tmp_reason.clause();

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
#if DEBUG_CAPTURE
	  cout << "  " << q << "  @" << var_level;
#endif
	  set_mark_and_putq(var);
	  bump_var_activity(var);
	  if ( var_level < decision_level() ) {
	    learnt.push_back(q);
#if DEBUG_CAPTURE
	    cout << "  ==>  learnt" << endl;
#endif
	  }
	  else {
	    ++ count;
#if DEBUG_CAPTURE
	    cout << "  ==>  QUEUE" << endl;
#endif
	  }
	}
      }
    }
    else {
      ASSERT_COND( !first );
      Literal q = tmp_reason.literal();
      VarId var = q.varid();
      int var_level = decision_level(var);
      if ( !get_mark(var) && var_level > 0 ) {
#if DEBUG_CAPTURE
	cout << "  " << q << "  @" << var_level;
#endif
	set_mark_and_putq(var);
	bump_var_activity(var);
	if ( var_level < decision_level() ) {
	  learnt.push_back(q);
#if DEBUG_CAPTURE
	  cout << "  ==>  learnt" << endl;
#endif
	}
	else {
	  ++ count;
#if DEBUG_CAPTURE
	  cout << "  ==>  QUEUE" << endl;
#endif
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
#if DEBUG_CAPTURE
	cout << "  <== " << q << endl;
#endif
	set_mark(var, false);
	-- last;
	-- count;
	if ( count > 0 ) {
	  tmp_reason = reason(q.varid());
	}
	else {
	  // それを最初のリテラルにする．
	  learnt[0] = ~q;
#if DEBUG_CAPTURE
	  cout << " UIP found" << endl;
#endif
	}
	break;
      }
      ASSERT_COND(last > 0 );
    }
    if ( count == 0 ) {
      // q は first UIP だった．
      break;
    }
  }

#if DEBUG_CAPTURE
  cout << "original learnt lits = ";
  for (ymuint i = 0; i < learnt.size(); ++ i) {
    cout << " " << learnt[i]
	 << " @" << decision_level(learnt[i].varid());
  }
  cout << endl;
#endif
}

END_NAMESPACE_YM_NLINK
