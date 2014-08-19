
/// @file SaBase.cc
/// @brief SaBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SaBase.h"
#include "YmSat.h"
#include "SatClause.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
// @param[in] solver SATソルバ
SaBase::SaBase(YmSat* solver) :
  SatAnalyzer(solver)
{
}

// @brief デストラクタ
SaBase::~SaBase()
{
}

// @brief 新しい変数が追加されたときに呼ばれる仮想関数
void
SaBase::alloc_var(ymuint size)
{
  mMark.resize(size, false);
}

// 再帰的なチェックを行う簡単化
// lit_list に含まれるリテラルのうち，その理由となっている割り当て，
// もしくはそのまた理由となっている割り当てを再帰的にたどり，
// そのなかに一つでもマークの付いていない決定割り当て(理由をもたない割り当て)
// があればそのリテラルを残す．
// 要するに，矛盾の起きた割り当てに対する極小セパレーター集合を求めている．
void
SaBase::make_minimal(vector<Literal>& lit_list)
{
  ymuint nl = lit_list.size();

  // lit_list に含まれているリテラルのレベルのビットマップ
  // ただし 64 のモジュロをとっている．
  ymuint64 lmask = 0UL;
  for (ymuint i = 0; i < nl; ++ i) {
    Literal p = lit_list[i];
    int level = decision_level(p.varid());
    lmask |= (1UL << (level & 63));
  }

  ymuint wpos = 0;
  for (ymuint i = 0; i < nl; ++ i) {
    Literal p = lit_list[i];
    VarId var = p.varid();
    ymuint top = mClearQueue.size();
    if ( check_recur(var, lmask) ) {
      if ( wpos != i ) {
	lit_list[wpos] = p;
      }
      ++ wpos;
    }
    for (ymuint j = top; j < mClearQueue.size(); ++ j) {
      set_mark(mClearQueue[j], false);
    }
    mClearQueue.erase(mClearQueue.begin() + top, mClearQueue.end());
  }
  if ( wpos < nl ) {
    lit_list.erase(lit_list.begin() + wpos, lit_list.end());
  }
}

// make_minimal のサブルーティン
// varid が極小セパレータの要素であるとき true を返す．
// 探索でたどったノードにはマークを付け，mClearQueue2 に積む．
// マークの付いたノードは2度と探索する必要はない．
// 以前の探索の結果が true ならその場で再帰関数は終わるので2度と
// たどることはないし，以前の結果が false ならそのままでよい．
bool
SaBase::check_recur(VarId varid,
		    ymuint64 lmask)
{
  mVarStack.clear();
  mVarStack.push_back(varid);

  while ( !mVarStack.empty() ) {
    varid = mVarStack.back();
    mVarStack.pop_back();
    SatReason r = reason(varid);
    if ( r == kNullSatReason ) {
      // varid は決定ノードだった．
      return true;
    }

    if ( ((1UL << (decision_level(varid) & 63)) & lmask) == 0UL ) {
      // varid の割り当てレベルと同じレベルの割り当ては lit_list に含まれていない．
      // ということはこのまま再帰を続けても，lit_list の他のリテラルには
      // 到達不可能であることがわかる．
      return true;
    }

    if ( r.is_clause() ) {
      SatClause* clause = r.clause();
      ymuint n = clause->lit_num();
      Literal p = clause->wl0();
      for (ymuint i = 0; i < n; ++ i) {
	Literal q = clause->lit(i);
	if ( q != p ) {
	  put_var(q);
	}
      }
    }
    else {
      Literal q = r.literal();
      put_var(q);
    }
  }
  return false;
}

// decision level の高いリテラルを 2番め (lit_list[1] の位置) に持ってくる
// 2番めのリテラルのレベルを返す．
int
SaBase::reorder(vector<Literal>& lit_list)
{
  ymuint n = lit_list.size();
  if ( n < 2 ) {
    return 0;
  }
  Literal lit1 = lit_list[1];
  int level = decision_level(lit1.varid());
  ymuint pos = 1;
  for (ymuint i = 2; i < n; ++ i) {
    Literal lit2 = lit_list[i];
    int level2 = decision_level(lit2.varid());
    if ( level < level2 ) {
      level = level2;
      pos = i;
    }
  }
  if ( pos != 1 ) {
    lit_list[1] = lit_list[pos];
    lit_list[pos] = lit1;
  }
  return level;
}

// var->mMark を設定してキューに積む
void
SaBase::set_mark_and_putq(VarId var)
{
  set_mark(var, true);
  mClearQueue.push_back(var);
}

// mClearQueue につまれた変数のマークを消す．
void
SaBase::clear_marks()
{
  // var->mMark をクリアする．
  for (vector<VarId>::iterator p = mClearQueue.begin();
       p != mClearQueue.end(); ++ p) {
    VarId var = *p;
    set_mark(var, false);
  }
  mClearQueue.clear();
}

END_NAMESPACE_YM_SAT
