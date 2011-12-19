
/// @file bmm_sup.cc
/// @brief サポート変数を求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrModern.h"
#include "BmmCompTbl.h"
#include "BmmVar.h"


BEGIN_NAMESPACE_YM_BDD

// edge_list に含まれる枝を根とするBDDのサポートに印をつける．
ymuint
BddMgrModern::mark_support(const vector<BddEdge>& edge_list)
{
  clear_varmark();
  // サポート変数にマークをつける．
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    sup_step(*p);
  }
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    clear_mark(e);
  }
  return mVarSet.size();
}

// サポート変数に印をつける．
void
BddMgrModern::sup_step(BddEdge e)
{
  for ( ; ; ) {
    BddNode* vp = e.get_node();
    if ( vp == 0 || vp->pmark() ) {
      return;
    }

    vp->pmark(1);
    ymuint level = vp->level();
    BmmVar* v = var_at(level);
    if ( !v->mMark ) {
      v->mMark = 1;
      mVarSet.push_back(v);
    }
    sup_step(vp->edge0());
    e = vp->edge1();
  }
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
BddMgrModern::mark_to_vector(VarVector& vars)
{
  ymuint n = mVarSet.size();
  vars.clear();
  vars.reserve(n);
  for (list<BmmVar*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    BmmVar* v = *p;
    VarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  sort(vars.begin(), vars.end());
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
ymuint
BddMgrModern::mark_to_list(VarList& vars)
{
  vars.clear();
  for (list<BmmVar*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    BmmVar* v = *p;
    VarId varid = v->varid();
    vars.push_back(varid);
  }
  clear_varmark();
  vars.sort();
  return vars.size();
}

// var_mark を列挙してマークのついた変数を vars に入れる．
BddEdge
BddMgrModern::mark_to_bdd()
{
  if ( mVarSet.empty() ) {
    return BddEdge::make_one();
  }
  list<BmmVar*>::iterator p = mVarSet.begin();
  BddEdge tmp = make_posiliteral((*p)->varid());
  for (++ p; p != mVarSet.end(); ++ p) {
    tmp = and_op(tmp, make_posiliteral((*p)->varid()));
  }
  clear_varmark();
  return tmp;
}

// Smallest Cube Containing F を求める．
// メモリ不足のためにエラーとなる可能性がある．
BddEdge
BddMgrModern::SCC(BddEdge e)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e.is_one() ) {
    return BddEdge::make_one();
  }
  if ( e.is_zero() ) {
    return BddEdge::make_zero();
  }

  // まずサポートを求める．
  mark_support(vector<BddEdge>(1, e));
  BddEdge sup = mark_to_bdd();

  // サポートを使って SCC を求める．
  clear_varmark();
  scc_step(e, sup);
  clear_mark(e);

  BddEdge ans = BddEdge::make_one();
  for (list<BmmVar*>::iterator p = mVarSet.begin(); p != mVarSet.end(); ++ p) {
    BmmVar* v = *p;
    if ( v->mMark == 1 ) {
      ans = and_op(ans, make_negaliteral(v->varid()));
    }
    else if ( v->mMark == 2 ) {
      ans = and_op(ans, make_posiliteral(v->varid()));
    }
  }
  clear_varmark();

  return ans;
}

// Smallest Cube Containing F を求めるためのサブルーティン．
void
BddMgrModern::scc_step(BddEdge e,
		       BddEdge s)
{
  if ( e.is_one() ) {
    while ( !s.is_one() ) {
      assert_cond(!s.is_zero(), __FILE__, __LINE__);
      BddNode* svp = s.get_node();
      ymuint slevel = svp->level();
      BmmVar* svar = var_at(slevel);
      svar->mMark |= 3;
      s = svp->edge1();
    }
    return;
  }

  BddNode* vp = e.get_node();
  if ( vp && !e.mark() ) {
    e.set_mark();
    ymuint level = vp->level();
    BmmVar* var = var_at(level);
    BddNode* svp = s.get_node();
    ymuint slevel = svp->level();
    BmmVar* svar = var_at(slevel);
    BddEdge s2 = svp->edge1();
    if ( svar->level() < var->level() ) {
      svar->mMark |= 3;
      scc_step(e, s2);
    }
    else {
      tPol pol = e.pol();
      BddEdge e0 = vp->edge0(pol);
      BddEdge e1 = vp->edge1(pol);
      if ( !var->mMark ) {
	mVarSet.push_back(var);
      }
      if ( !e0.is_zero() ) {
	var->mMark |= 1;
	scc_step(e0, s2);
      }
      if ( !e1.is_zero() ) {
	var->mMark |= 2;
	scc_step(e1, s2);
      }
    }
  }
}

// existential quntification(smoothing)
// svars に含まれる変数を消去する．
// メモリ不足のためにエラーとなる可能性がある．
BddEdge
BddMgrModern::esmooth(BddEdge e1,
		      BddEdge e2)
{
  if ( e1.is_error() || e2.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  clear_varmark();
  mLastLevel = 0;
  {
    BddNode* vp = e2.get_node();
    while ( vp != 0 ) {
      ymuint level = vp->level();
      BmmVar* v = var_at(level);
      mLastLevel = level;
      v->mMark = 1;
      vp = vp->edge1().get_node();
    }
  }
  BddEdge ans = esmooth_step(e1);
  mSmTable->clear();
  clear_varmark();
  return ans;
}

BddEdge
BddMgrModern::esmooth_step(BddEdge e)
{
  if ( e.is_leaf() ) {
    return e;
  }

  BddNode* vp = e.get_node();
  ymuint level = vp->level();
  if ( level > mLastLevel ) {
    return e;
  }

  BddEdge result = mSmTable->get(e);
  if ( result.is_error() ) {
    tPol pol = e.pol();
    BddEdge e0 = vp->edge0(pol);
    BddEdge e1 = vp->edge1(pol);
    BmmVar* var = var_at(level);
    if ( var->mMark ) {
      // 消去対象の変数だった．
      BddEdge tmp = or_op(e0, e1);
      result = esmooth_step(tmp);
    }
    else {
      BddEdge r_0 = esmooth_step(e0);
      BddEdge r_1 = esmooth_step(e1);
      result = new_node(level, r_0, r_1);
    }
    mSmTable->put(e, result);
  }

  return result;
}

// src1 と src2 との論理積を計算しつつスムージングを行う．
BddEdge
BddMgrModern::and_exist(BddEdge e1,
			BddEdge e2,
			BddEdge e3)
{
  if ( e1.is_error() || e2.is_error() || e3.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() || e3.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  clear_varmark();
  mLastLevel = 0;
  {
    BddNode* vp = e3.get_node();
    while ( vp != 0 ) {
      mLastLevel = vp->level();
      BmmVar* v = var_at(mLastLevel);
      v->mMark = 1;
      vp = vp->edge1().get_node();
    }
  }
  BddEdge ans = andexist_step(e1, e2);
  mSmTable->clear();
  mAeTable->clear();
  clear_varmark();
  return ans;
}

BddEdge
BddMgrModern::andexist_step(BddEdge f,
			    BddEdge g)
{
  if ( f.is_zero() || g.is_zero() ) {
    return BddEdge::make_zero();
  }
  if ( f.is_one() && g.is_one() ) {
    return BddEdge::make_one();
  }
  if ( f.is_one() ) {
    return esmooth_step(g);
  }
  if ( g.is_one() ) {
    return esmooth_step(f);
  }

  if ( f > g ) {
    BddEdge tmp = f;
    f = g;
    g = tmp;
  }

  BddNode* f_vp = f.get_node();
  BddNode* g_vp = g.get_node();
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint level = f_level;
  if ( level > g_level ) {
    level = g_level;
  }
  if ( level > mLastLevel ) {
    return and_op(f, g);
  }

  BddEdge result = mAeTable->get(f, g);
  if ( result.is_error() ) {
    BddEdge f_0, f_1;
    BddEdge g_0, g_1;
    BmmVar* var = var_at(level);
    if ( var->mMark ) {
      if ( f_level > level ) {
	g_0 = g_vp->edge0(g_pol);
	g_1 = g_vp->edge1(g_pol);
	BddEdge tmp = or_op(g_0, g_1);
	result = andexist_step(f, tmp);
      }
      else if ( g_level > level ) {
	f_0 = f_vp->edge0(f_pol);
	f_1 = f_vp->edge1(f_pol);
	BddEdge tmp = or_op(f_0, f_1);
	result = andexist_step(tmp, g);
      }
      else {
	f_0 = f_vp->edge0(f_pol);
	f_1 = f_vp->edge1(f_pol);
	g_0 = g_vp->edge0(g_pol);
	g_1 = g_vp->edge1(g_pol);
	BddEdge tmp1 = andexist_step(f_0, g_0);
	BddEdge tmp2 = andexist_step(f_1, g_1);
	result = or_op(tmp1, tmp2);
      }
    }
    else {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
      BddEdge r_0 = andexist_step(f_0, g_0);
      BddEdge r_1 = andexist_step(f_1, g_1);
      result = new_node(level, r_0, r_1);
    }
    mAeTable->put(f, g, result);
  }

  return result;
}

END_NAMESPACE_YM_BDD
