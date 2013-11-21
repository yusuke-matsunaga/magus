
/// @file bmm_ite.cc
/// @brief ITE 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrModern.h"
#include "BmmCompTbl.h"
#include "BmmVar.h"


BEGIN_NAMESPACE_YM_BDD

// multiple compose 演算を行うために最初に呼ばれる関数．
void
BddMgrModern::compose_start()
{
  // 念のため
  clear_varmark();
  mLastLevel = 0;
}

// multiple compose 演算を行う変数と置き換え関数を登録する関数
void
BddMgrModern::compose_reg(VarId id,
			  BddEdge e)
{
  BmmVar* var = var_of(id);
  if ( var ) {
    var->mMark = 1;
    var->mCompEdge = e;
    ymuint level = var->level();
    if ( mLastLevel < level ) {
      mLastLevel = level;
    }
  }
}

// multiple compose 演算の本体
BddEdge
BddMgrModern::compose(BddEdge e)
{
  BddEdge ans = compose_step(e);
  clear_varmark();
  mCmpTable->clear();
  return ans;
}

BddEdge
BddMgrModern::compose_step(BddEdge f)
{
  if ( f.is_error() ) {
    return BddEdge::make_error();
  }
  if ( f.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( f.is_leaf() ) {
    return f;
  }

  BddNode* f_vp = f.get_node();
  ymuint f_level = f_vp->level();
  if ( f_level > mLastLevel ) {
    return f;
  }

  // 極性の反転に関して線形性を有するので極性は取り除く．
  tPol f_pol = f.pol();
  f.normalize();

  BddEdge result = mCmpTable->get(f);
  if ( result.is_error() ) {
    BddEdge f_0 = f_vp->edge0();
    BddEdge f_1 = f_vp->edge1();
    BddEdge r_0 = compose_step(f_0);
    BddEdge r_1 = compose_step(f_1);
    BddEdge tmp;
    BmmVar* f_var = var_at(f_level);
    if ( f_var->mMark == 1 ) {
      tmp = f_var->mCompEdge;
    }
    else {
      tmp = new_node(f_level, BddEdge::make_zero(), BddEdge::make_one());
    }
    result = ite_op(tmp, r_1, r_0);
    mCmpTable->put(f, result);
  }

  return BddEdge(result, f_pol);
}

// x_level の変数を y_level まで「押し込む」．
// pol が kPolNega の時は 0-枝と 1-枝を取り替える．
BddEdge
BddMgrModern::push_down(BddEdge e,
			ymuint x_level,
			ymuint y_level,
			tPol pol)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }

#if 0
  y_edge = BddMgr::make_bdd(varid(y_level), BddEdge::make_zero(), BddEdge::make_one());
  xy_edge = BddMgr::make_bdd(varid(x_level), BddEdge::make_zero(), addpol(y_edge, pol));
  activate(xy_edge);
  BddEdge ans = pd_step(e, x_level, y_level, pol);
  deactivate(xy_edge);
  return ans;
#else
  return BddEdge::make_zero();
#endif
}

END_NAMESPACE_YM_BDD
