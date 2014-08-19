
/// @file bmc_ite.cc
/// @brief ITE 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"
#include "BmcVar.h"
#include "BmcCompTbl.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

BddEdge y_edge;
BddEdge xy_edge;

END_NONAMESPACE

// multiple compose 演算を行うために最初に呼ばれる関数．
void
BddMgrClassic::compose_start()
{
  // 念のため
  clear_varmark();
  mLastLevel = 0;
}

// multiple compose 演算を行う変数と置き換え関数を登録する関数
void
BddMgrClassic::compose_reg(VarId id,
			   BddEdge e)
{
  Var* var = var_of(id);
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
BddMgrClassic::compose(BddEdge e)
{
  BddEdge ans = compose_step(e);
  clear_varmark();
  mCmpTable->clear();
  return ans;
}

BddEdge
BddMgrClassic::compose_step(BddEdge f)
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
  bool f_inv = f.inv();
  f.normalize();

  BddEdge result = mCmpTable->get(f);
  if ( result.is_error() ) {
    BddEdge f_0 = f_vp->edge0();
    BddEdge f_1 = f_vp->edge1();
    BddEdge r_0 = compose_step(f_0);
    if ( !r_0.is_invalid() ) {
      BddEdge r_1 = compose_step(f_1);
      if ( !r_1.is_invalid() ) {
	BddEdge tmp;
	Var* f_var = var_at(f_level);
	if ( f_var->mMark == 1 ) {
	  tmp = f_var->mCompEdge;
	}
	else {
	  tmp = new_node(f_level, BddEdge::make_zero(), BddEdge::make_one());
	}
	result = ite_op(tmp, r_1, r_0);
	mCmpTable->put(f, result);
      }
    }
  }

  return BddEdge(result, f_inv);
}

// x_level の変数を y_level まで「押し込む」．
// pol が kPolNega の時は 0-枝と 1-枝を取り替える．
BddEdge
BddMgrClassic::push_down(BddEdge e,
			 ymuint x_level,
			 ymuint y_level,
			 bool inv)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }

  y_edge = make_posiliteral(varid(y_level));
  BddEdge x_edge = make_posiliteral(varid(x_level));
  xy_edge = and_op(x_edge, y_edge);
  activate(xy_edge);
  BddEdge ans = pd_step(e, x_level, y_level, inv);
  deactivate(xy_edge);
  return ans;
}

BddEdge
BddMgrClassic::pd_step(BddEdge e,
		       ymuint x_level,
		       ymuint y_level,
		       bool inv)
{
  if ( e.is_leaf() ) {
    return e;
  }

  BddNode* vp = e.get_node();
  if ( vp->level() > y_level ) {
    // 変化なし
    return e;
  }
  if ( vp->level() > x_level ) {
    // 一つ繰り上げる．
    return pd_step2(e, y_level);
  }

  // 極性を正規化しておく．
  bool e_inv = e.inv();
  e.normalize();
  BddEdge result = mPushDownTable->get(e, xy_edge);
  if ( result.is_error() ) {
    if ( vp->level() == x_level ) {
      BddEdge e0 = vp->edge0();
      BddEdge e1 = vp->edge1();
      result = pd_step3(e0, e1, y_level, inv);
    }
    else {
      BddEdge e0 = vp->edge0();
      BddEdge e1 = vp->edge1();
      BddEdge r0 = pd_step(e0, x_level, y_level, inv);
      if ( !r0.is_invalid() ) {
	BddEdge r1 = pd_step(e1, x_level, y_level, inv);
	if ( !r1.is_invalid() ) {
	  result = new_node(vp->level(), r0, r1);
	  mPushDownTable->put(e, xy_edge, result);
	}
      }
    }
  }
  return BddEdge(result, e_inv);
}

BddEdge
BddMgrClassic::pd_step2(BddEdge e,
			ymuint y_level)
{
  if ( e.is_leaf() ) {
    return e;
  }

  BddNode* vp = e.get_node();
  if ( vp->level() > y_level ) {
    return e;
  }

  // 極性を正規化しておく．
  bool e_inv = e.inv();
  e.normalize();
  BddEdge result = mPushDownTable2->get(e, xy_edge);
  if ( result.is_error() ) {
    BddEdge e0 = vp->edge0();
    BddEdge e1 = vp->edge1();
    BddEdge r0 = pd_step2(e0, y_level);
    if ( !r0.is_invalid() ) {
      BddEdge r1 = pd_step2(e1, y_level);
      if ( !r1.is_invalid() ) {
	VarId vid = varid(vp->level() - 1);
	Var* var = var_of(vid);
	if ( !var ) {
	  var = alloc_var(vid);
	}
	result = new_node(var->level(), r0, r1);
	mPushDownTable2->put(e, xy_edge, result);
      }
    }
  }
  return BddEdge(result, e_inv);
}

BddEdge
BddMgrClassic::pd_step3(BddEdge e0,
			BddEdge e1,
			ymuint y_level,
			bool inv)
{
  BddNode* vp0 = e0.get_node();
  BddNode* vp1 = e1.get_node();
  bool inv0 = e0.inv();
  bool inv1 = e1.inv();
  ymuint top_level = kLevelMax;
  if ( vp0 && top_level > vp0->level() ) {
    top_level = vp0->level();
  }
  if ( vp1 && top_level > vp1->level() ) {
    top_level = vp1->level();
  }

  BddEdge result = mPushDownTable3->get(e0, e1, xy_edge);
  if ( result.is_error() ) {
    if ( top_level > y_level ) {
      VarId vid = varid(y_level);
      Var* var = var_of(vid);
      if ( !var ) {
	var = alloc_var(vid);
      }
      if ( inv ) {
	result = new_node(y_level, e1, e0);
      }
      else {
	result = new_node(y_level, e0, e1);
      }
    }
    else {
      BddEdge e00, e01;
      BddEdge e10, e11;
      if ( vp0 && top_level == vp0->level() ) {
	e00 = vp0->edge0(inv0);
	e01 = vp0->edge1(inv0);
      }
      else {
	e00 = e01 = e0;
      }
      if ( vp1 && top_level == vp1->level() ) {
	e10 = vp1->edge0(inv1);
	e11 = vp1->edge1(inv1);
      }
      else {
	e10 = e11 = e1;
      }
      BddEdge r0 = pd_step3(e00, e10, y_level, inv);
      if ( !r0.is_invalid() ) {
	BddEdge r1 = pd_step3(e01, e11, y_level, inv);
	if ( !r1.is_invalid() ) {
	  VarId vid = varid(top_level - 1);
	  Var* var = var_of(vid);
	  if ( !var ) {
	    var = alloc_var(vid);
	  }
	  result = new_node(var->level(), r0, r1);
	  mPushDownTable3->put(e0, e1, xy_edge, result);
	}
      }
    }
  }
  return result;
}

END_NAMESPACE_YM_BDD
