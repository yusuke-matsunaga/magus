
/// @file bmc_sup.cc
/// @brief サポート変数を求める関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcVar.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

#if 0
// existential quntification(smoothing)
// svars に含まれる変数を消去する．
// メモリ不足のためにエラーとなる可能性がある．
BddEdge
BddMgrClassic::esmooth(BddEdge e1,
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
      mLastLevel = vp->level();
      Var* var = var_at(mLastLevel);
      var->mMark = 1;
      vp = vp->edge1().get_node();
    }
  }
  BddEdge ans = esmooth_step(e1);
  mSmTable->clear();
  clear_varmark();
  return ans;
}

BddEdge
BddMgrClassic::esmooth_step(BddEdge e)
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
    Var* var = var_at(level);
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
#endif

// src1 と src2 との論理積を計算しつつスムージングを行う．
BddEdge
BddMgrClassic::and_exist(BddEdge e1,
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
      Var* var = var_at(mLastLevel);
      var->mMark = 1;
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
BddMgrClassic::andexist_step(BddEdge f,
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
    Var* var = var_at(level);
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
