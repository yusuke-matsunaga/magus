
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

// f が真の時 g を，偽の時 h を選ぶ関数
BddEdge
BddMgrModern::ite_op(BddEdge f,
		     BddEdge g,
		     BddEdge h)
{
  if ( f.is_error() || g.is_error() || h.is_error() ) {
    return BddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() || h.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( f.is_one() ) {
    return g;
  }
  if ( f.is_zero() ) {
    return h;
  }
  if ( g == h ) {
    return g;
  }
  if ( check_reverse(g, h) ) {
    return xor_op(f, h);
  }
  if ( g.is_one() || f == g ) {
    // f | h
    return or_op(f, h);
  }
  if ( g.is_zero() || check_reverse(f, g) ) {
    // ~f & h
    return and_op(~f, h);
  }
  if ( h.is_one() || check_reverse(f, h) ) {
    // ~f | g
    return or_op(~f, g);
  }
  if ( h.is_zero() || f == h ) {
    // f & g
    return and_op(f, g);
  }
  // この時点で f, g, h は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する．
  if ( g > h ) {
    BddEdge tmp = g;
    g = h;
    h = tmp;
    // then と else を入れ替えたんだから条件も反転しなければならない．
    f.negate();
  }

  // さらに g に否定属性を付けないように正規化する．
  tPol ans_pol = g.pol();
  g.addpol(ans_pol);
  h.addpol(ans_pol);

  BddNode* f_vp = f.get_node();
  BddNode* g_vp = g.get_node();
  BddNode* h_vp = h.get_node();
  ymuint f_level = f_vp->level();
  ymuint g_level = g_vp->level();
  ymuint h_level = h_vp->level();

  BddEdge result;

  tPol f_pol = f.pol();
  if ( f_vp->edge0(f_pol).is_zero() &&
       f_vp->edge1(f_pol).is_one() &&
       f_level < g_level && f_level < h_level ) {
    result = new_node(f_level, h, g);
  }
  else if ( f_vp->edge0(f_pol).is_one() &&
	    f_vp->edge1(f_pol).is_zero() &&
	    f_level < g_level && f_level < h_level ) {
    result = new_node(f_level, g, h);
  }
  else {
    result = mIteTable->get(f, g, h);
    if ( result.is_error() ) {
      tPol g_pol = g.pol();
      tPol h_pol = h.pol();
      ymuint top = f_level;
      if ( top > g_level) {
	top = g_level;
      }
      if ( top > h_level ) {
	top = h_level;
      }
      BddEdge f_0, f_1;
      BddEdge g_0, g_1;
      BddEdge h_0, h_1;
      split1(top, f_level, f, f_vp, f_pol, f_0, f_1);
      split1(top, g_level, g, g_vp, g_pol, g_0, g_1);
      split1(top, h_level, h, h_vp, h_pol, h_0, h_1);
      BddEdge r_0 = ite_op(f_0, g_0, h_0);
      BddEdge r_1 = ite_op(f_1, g_1, h_1);
      result = new_node(top, r_0, r_1);
      mIteTable->put(f, g, h, result);
    }
  }
  return BddEdge(result, ans_pol);
}

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

// generalized cofactor 演算
BddEdge
BddMgrModern::gcofactor(BddEdge e1,
			BddEdge e2)
{
  if ( e1.is_error() || e2.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  if ( e2.is_zero() ) {
    return BddEdge::make_error();
  }
  if ( check_cube(e2) ) {
    return cube_division(e1, e2);
  }
  return gcofactor_step(e1, e2);
}

// constrain演算
// 注: c はBddEdge::make_zero()であってはならない．
BddEdge
BddMgrModern::gcofactor_step(BddEdge f,
			     BddEdge c)
{
  assert_cond( !c.is_zero(), __FILE__, __LINE__);

  if ( f.is_leaf() || c.is_one() ) {
    return f;
  }
  if ( f == c ) {
    return BddEdge::make_one();
  }
  if ( check_reverse(f, c) ) {
    return BddEdge::make_zero();
  }
  // この時点で f, c は終端ではない．

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = f.pol();
  f.normalize();
  BddEdge result = mCofacTable->get(f, c);
  if ( result.is_error() ) {
    BddNode* f_v = f.get_node();
    BddNode* c_v = c.get_node();
    tPol c_p = c.pol();
    ymuint f_level = f_v->level();
    ymuint c_level = c_v->level();

    BddEdge f_0, f_1;
    if ( f_level <= c_level ) {
      f_0 = f_v->edge0();
      f_1 = f_v->edge1();
    }
    else {
      f_0 = f_1 = f;
    }

    if ( f_level < c_level ) {
      BddEdge r0 = gcofactor_step(f_0, c);
      BddEdge r1 = gcofactor_step(f_1, c);
      result = new_node(f_level, r0, r1);
    }
    else {
      BddEdge c_0 = c_v->edge0(c_p);
      BddEdge c_1 = c_v->edge1(c_p);
      if ( c_0.is_zero() ) {
	result = gcofactor_step(f_1, c_1);
      }
      else if ( c_1.is_zero() ) {
	result = gcofactor_step(f_0, c_0);
      }
      else {
	BddEdge r0 = gcofactor_step(f_0, c_0);
	BddEdge r1 = gcofactor_step(f_1, c_1);
	result = new_node(c_level, r0, r1);
      }
    }
    mCofacTable->put(f, c, result);
  }

  // 極性を元に戻す．
  result.addpol(f_pol);

  return result;
}

// 一つの変数に対する cofactor を計算する．
BddEdge
BddMgrModern::scofactor(BddEdge e1,
			VarId id,
			tPol pol)
{
  clear_varmark();
  BmmVar* var = var_of(id);
  if ( !var ) {
    // var がないということは e1 中に含まれていない
    return e1;
  }
  if ( pol == kPolPosi ) {
    var->mMark = 1;
  }
  else {
    var->mMark = 2;
  }
  mLastLevel = var->level();
  BddEdge ans = cubediv_step(e1);
  mCubedivTable->clear();
  clear_varmark();
  return ans;
}

// C がキューブの場合の拘束演算
BddEdge
BddMgrModern::cube_division(BddEdge f,
			    BddEdge c)
{
  clear_varmark();
  BddEdge e = c;
  BddNode* vp = e.get_node();
  tPol pol = e.pol();
  mLastLevel = 0;
  while ( vp != 0 ) {
    BddEdge e0 = vp->edge0(pol);
    BddEdge e1 = vp->edge1(pol);
    ymuint level = vp->level();
    BmmVar* var = var_at(level);
    if ( e0.is_zero() ) {
      var->mMark = 1;
      e = e1;
    }
    else {
      var->mMark = 2;
      e = e0;
    }
    mLastLevel = level;
    vp = e.get_node();
    pol = e.pol();
  }
  BddEdge ans = cubediv_step(f);
  mCubedivTable->clear();
  clear_varmark();
  return ans;
}

// restricter がキューブの時は必ず O(n):nは第一引数のBDDのサイズ
// でできる
BddEdge
BddMgrModern::cubediv_step(BddEdge f)
{
  if ( f.is_leaf() ) {
    return f;
  }
  // この時点で f, g は終端ではない．

  BddNode* f_vp = f.get_node();
  ymuint f_level = f_vp->level();
  if ( f_level > mLastLevel ) {
    return f;
  }

  // (~f) / c の結果は ~(f / c) なので f を正規化する．
  tPol f_pol = f.pol();
  f.normalize();

  BddEdge result = mCubedivTable->get(f);
  if ( result.is_error() ) {
    BmmVar* f_var = var_at(f_level);
    if ( f_var->mMark == 1 ) {
      // 肯定のリテラルが現れているので 1枝の結果を返す．
      BddEdge f_1 = f_vp->edge1();
      result = cubediv_step(f_1);
    }
    else if ( f_var->mMark == 2 ) {
      // 否定のリテラルが現れているので 0枝の結果を返す．
      BddEdge f_0 = f_vp->edge0();
      result = cubediv_step(f_0);
    }
    else {
      // この変数はキューブには現れない
      BddEdge f_0 = f_vp->edge0();
      BddEdge f_1 = f_vp->edge1();

      BddEdge r_0 = cubediv_step(f_0);
      BddEdge r_1 = cubediv_step(f_1);

      result = new_node(f_level, r_0, r_1);
    }
    mCubedivTable->put(f, result);
  }

  // 極性を元に戻す．
  result.addpol(f_pol);

  return result;
}

// Davio展開のモーメント項($f_{\overline{x}} \oplus f_x$)を
// 求める処理
// メモリ不足のためにエラーとなる可能性がある．
BddEdge
BddMgrModern::xor_moment(BddEdge e,
			 VarId idx)
{
  if ( e.is_error() ) {
    return BddEdge::make_error();
  }
  if ( e.is_overflow() ) {
    return BddEdge::make_overflow();
  }
  BmmVar* var = var_of(idx);
  if ( !var ) {
    // この変数がないということは答は0
    return BddEdge::make_zero();
  }
  mLastLevel = var->level();
  BddEdge ans = xcofactor_step(e);
  mXcofactorTable->clear();
  return ans;
}

// Davio 展開もモーメント項を求める．
BddEdge
BddMgrModern::xcofactor_step(BddEdge f)
{
  if ( f.is_leaf() ) {
    // 終端ならば x でコファクターをとっても違いがないので
    // 答は 0 となる．
    return BddEdge::make_zero();
  }

  // この時点で e は終端ではない．
  BddNode* node = f.get_node();
  ymuint level = node->level();
  if ( level > mLastLevel ) {
    return BddEdge::make_zero();
  }

  // f の結果と ~f の結果は等しいので
  // 極性を正規化する．
  f.normalize();

  // まずハッシュを探す．
  BddEdge result = mXcofactorTable->get(f);
  if ( result.is_error() ) {
    // ハッシュになかった．
    BddEdge e_0 = node->edge0();
    BddEdge e_1 = node->edge1();
    if ( level == mLastLevel ) {
      result = xor_op(e_0, e_1);
    }
    else { // level < mLastLevel
      BddEdge r_0 = xcofactor_step(e_0);
      BddEdge r_1 = xcofactor_step(e_1);
      result = new_node(level, r_0, r_1);
    }
    // ハッシュに登録しておく．
    mXcofactorTable->put(f, result);
  }
  return result;
}

END_NAMESPACE_YM_BDD
