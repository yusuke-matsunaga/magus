
/// @file cap_op.cc
/// @brief cap 演算を行う関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ZddMgrImpl.h"
#include "CompTbl.h"


BEGIN_NAMESPACE_YM_ZDD

// e1 $\cap$ e2 を計算する．
ZddEdge
ZddMgrImpl::cap_op(ZddEdge f,
		   ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  return cap_step(f, g);
}

// cap_op の下請け関数
ZddEdge
ZddMgrImpl::cap_step(ZddEdge f,
		     ZddEdge g)
{
  // 0-element 属性に対するルール
  // f, g ともに 0-element 属性をもっていたら答にも 0-element 属性を持たせる．
  bool zattr = f.zattr() & g.zattr();
  f.normalize();
  g.normalize();

  ZddEdge ans_e;

  // 特別な場合の処理
  // 1：片方のZDDが0なら答えは0，
  // 2：同じZDDどうしのANDは自分自身
  if ( f.is_zero() || g.is_zero() ) {
    ans_e = ZddEdge::make_zero();
  }
  else if ( f == g ) {
    ans_e = f;
  }
  else {
    // この時点で f,g は終端ではない．

    // 演算結果テーブルが当たりやすくなるように順序を正規化する
    if ( f > g ) {
      ZddEdge tmp = f;
      f = g;
      g = tmp;
    }

    ZddEdge result = mCapTable->get(f, g);
    if ( result.is_error() ) {
      // 演算結果テーブルには登録されていない
      ZddEdge f_0, f_1;
      ZddEdge g_0, g_1;
      ZddVar* var = split(f, g, f_0, f_1, g_0, g_1);
      ZddEdge r_0 = cap_step(f_0, g_0);
      if ( r_0.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ZddEdge r_1 = cap_step(f_1, g_1);
      if ( r_1.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      result = new_node(var, r_0, r_1);
      mCapTable->put(f, g, result);
    }
  }
  return ans_e.add_zattr(zattr);
}

// e1 $\cup$ e2 を計算する．
ZddEdge
ZddMgrImpl::cup_op(ZddEdge f,
		   ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  return cup_step(f, g);
}

// cup_op の下請け関数
ZddEdge
ZddMgrImpl::cup_step(ZddEdge f,
		     ZddEdge g)
{
  // 0-element 属性に対するルール
  // f, g のどちらかが 0-element 属性をもっていたら答にも 0-element 属性を持たせる．
  bool zattr = f.zattr() | g.zattr();
  f.normalize();
  g.normalize();

  ZddEdge ans_e;

  // 特別な場合の処理
  // 1：片方のZDDが0なら答えは他方，
  // 2：同じZDDどうしのANDは自分自身
  if ( f.is_zero() ) {
    ans_e = g;
  }
  else if ( g.is_zero() || f == g ) {
    ans_e = f;
  }
  else {
    // この時点で f,g は終端ではない．

    // 演算結果テーブルが当たりやすくなるように順序を正規化する
    if ( f > g ) {
      ZddEdge tmp = f;
      f = g;
      g = tmp;
    }

    ZddEdge result = mCupTable->get(f, g);
    if ( result.is_error() ) {
      // 演算結果テーブルには登録されていない
      ZddEdge f_0, f_1;
      ZddEdge g_0, g_1;
      ZddVar* var = split(f, g, f_0, f_1, g_0, g_1);
      ZddEdge r_0 = cup_step(f_0, g_0);
      if ( r_0.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ZddEdge r_1 = cup_step(f_1, g_1);
      if ( r_1.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      result = new_node(var, r_0, r_1);
      mCapTable->put(f, g, result);
    }
  }
  return ans_e.add_zattr(zattr);
}

// 2つのZDDのdiffを計算するapply演算
ZddEdge
ZddMgrImpl::diff_op(ZddEdge f,
		    ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  return diff_step(f, g);
}

// diff_op の下請け関数
ZddEdge
ZddMgrImpl::diff_step(ZddEdge f,
		      ZddEdge g)
{
  // 0-element 属性に関するルール
  // g に 0-element 属性があったら答から属性を取り去る．
  bool zattr = f.zattr();
  if ( g.zattr() ) {
    zattr = false;
  }

  f.normalize();
  g.normalize();

  ZddEdge ans_e;

  // 特別な場合の処理
  // 1: fが0なら0を返す．
  // 2: gが0ならfを返す．
  // 3: おなじノードなら0を返す．
  // 4: 極性のみが異なる関数同士なら1を返す．
  if ( f.is_zero() || g.is_zero() ) {
    ans_e = f;
  }
  else if ( f == g ) {
    ans_e = ZddEdge::make_zero();
  }
  else {
    // この時点で f, g は終端ではない．

    ZddEdge result = mDiffTable->get(f, g);
    if ( result.is_error() ) {
      ZddEdge f_0, f_1;
      ZddEdge g_0, g_1;
      ZddVar* var = split(f, g, f_0, f_1, g_0, g_1);
      ZddEdge r_0 = diff_step(f_0, g_0);
      if ( r_0.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      ZddEdge r_1 = diff_step(f_1, g_1);
      if ( r_1.is_overflow() ) {
	return ZddEdge::make_overflow();
      }
      result = new_node(var, r_0, r_1);
      mDiffTable->put(f, g, result);
    }
  }
  return ans_e.add_zattr(zattr);
}

#if 0
// 2つの論理関数が交わっていれば kEdge1 を返す．
ZddEdge
ZddMgrImpl::check_intersect(ZddEdge f,
			    ZddEdge g)
{
  // エラー状態のチェック
  if ( f.is_error() || g.is_error() ) {
    // どちらかがエラー
    return ZddEdge::make_error();
  }
  if ( f.is_overflow() || g.is_overflow() ) {
    // どちらかがオーバーフロー
    return ZddEdge::make_overflow();
  }

  // 特別な場合の処理
  // 1: 片方が0なら絶対に交わらないので 0 を返す．
  // 2: 極性のみが異なっている場合にも交わらないので 0 を返す．
  // 3: 片方が1なら交わっているので 1 を返す．
  // 4: 同じ関数なら交わっているので 1 を返す．
  if ( f.is_zero() || g.is_zero() || check_reverse(f, g) ) {
    return ZddEdge::make_zero();
  }
  if ( f.is_one() || g.is_one() || f == g ) {
    return ZddEdge::make_one();
  }
  // この時点で f, g は終端ではない．

  // 極性は正規化されている(否定属性は0枝のみ，葉は1のみ)
  // ので f も g も正極性のときは f(1,1,...,1) = g(1,1,...,1) = 1
  // つまりすべての入力を 1 にしたときの関数値は 1 であるので
  // 交わっていることがわかる．
  tPol f_pol = f.pol();
  tPol g_pol = g.pol();
  if ( f_pol == kPolPosi && g_pol == kPolPosi ) {
    return ZddEdge::make_one();
  }

  if ( f > g ) {
    ZddEdge tmp = f;
    f = g;
    g = tmp;
    tPol tmp_pol = f_pol;
    f_pol = g_pol;
    g_pol = tmp_pol;
  }

  ZddEdge result = mIntTable->get(f, g);
  if ( result.is_error() ) {
    Node* f_vp = get_node(f);
    Node* g_vp = get_node(g);
    ZddVar* f_var = f_vp->var();
    ZddVar* g_var = g_vp->var();
    tLevel f_level = f_var->level();
    tLevel g_level = g_var->level();
    ZddEdge f_0, f_1;
    ZddEdge g_0, g_1;
    if ( f_level < g_level ) {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_1 = g;
    }
    else if ( f_level > g_level ) {
      f_0 = f_1 = f;
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
    }
    else {
      f_0 = f_vp->edge0(f_pol);
      f_1 = f_vp->edge1(f_pol);
      g_0 = g_vp->edge0(g_pol);
      g_1 = g_vp->edge1(g_pol);
    }
    result = check_intersect(f_0, g_0);
    if ( result.is_zero() ) {
      result = check_intersect(f_1, g_1);
    }
    mIntTable->put(f, g, result);
  }
  return result;
}
#endif

END_NAMESPACE_YM_ZDD
