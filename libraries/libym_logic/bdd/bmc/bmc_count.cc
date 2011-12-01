
/// @file bmc_count.cc
/// @brief 節点数を数える関数の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "BddMgrClassic.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

mpz_class n_invect;
ymuint32 n_invect_int;

ymuint32 w_level;

END_NONAMESPACE

// e を根とするBDDの節点数を数える．
ymuint64
BddMgrClassic::size(BddEdge e)
{
  mNum = 0;
  count1(e);
  clear_pnmark(e);
  return mNum;
}

// edge list に登録されたBDDの節点数を数える．
ymuint64
BddMgrClassic::size(const list<BddEdge>& edge_list)
{
  mNum = 0;
  for (list<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    count1(*p);
  }
  for (list<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    clear_pnmark(*p);
  }
  return mNum;
}

// size() の下請関数
void
BddMgrClassic::count1(BddEdge e)
{
  for ( ; ; ) {
    Node* vp = get_node(e);
    if ( vp == 0 || vp->pmark() ) {
      return;
    }
    ++ mNum;
    vp->pmark(1);
    count1(vp->edge0());
    e = vp->edge1();
  }
}

// BDD の表す論理関数の minterm の数を返す．
// 無限長精度の整数(mpz_class)を用いて計算する．
// 論理関数の変数の数を指定するバージョン
mpz_class
BddMgrClassic::minterm_count(BddEdge e,
			     ymuint n)
{
  if ( e.is_overflow() ) {
    return 0;
  }
  if ( e.is_error() ) {
    return 0;
  }

  ymuint bitsize = n + 1;
  if ( bitsize < sizeof(ymuint) * 8 ) {
    // 値の範囲が int に収まるのなら int 版の関数を呼ぶ．

    // 全入力ベクトルの数の計算
    n_invect_int = 1U << n;

    hash_map<BddEdge, ymuint> mc_map;
    ymuint ans = mterm_step(e, mc_map);

    return mpz_class(ans);
  }
  else {
    // 全入力ベクトルの数の計算
    n_invect = mpz_class(1U) << n;

    hash_map<BddEdge, mpz_class> mc_map;
    mpz_class ans = mterm_step(e, mc_map);
    return ans;
  }
}

// mpz_class 版の minterm_count の下請関数
mpz_class
BddMgrClassic::mterm_step(BddEdge e,
			  hash_map<BddEdge, mpz_class>& mc_map)
{
  if ( e.is_one() ) {
    return n_invect;
  }
  if ( e.is_zero() ) {
    return 0;
  }

  Node* vp = get_node(e);
  tPol pol = e.pol();
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<BddEdge, mpz_class>::iterator p = mc_map.find(e);
    if ( p != mc_map.end() ) {
      return p->second;
    }
  }

  // 子ノードが表す関数のminterm数を計算する
  mpz_class n0 = mterm_step(vp->edge0(pol), mc_map);
  mpz_class n1 = mterm_step(vp->edge1(pol), mc_map);

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  mpz_class ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    mc_map[e] = ans;
  }

  return ans;
}

// int 版の minterm_count の下請関数
ymuint
BddMgrClassic::mterm_step(BddEdge e,
			  hash_map<BddEdge, ymuint>& mc_map)
{
  if ( e.is_one() ) {
    return n_invect_int;
  }
  if ( e.is_zero() ) {
    return 0;
  }

  Node* vp = get_node(e);
  tPol pol = e.pol();
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<BddEdge, ymuint>::iterator p = mc_map.find(e);
    if ( p != mc_map.end() ) {
      return p->second;
    }
  }


  // 子ノードが表す関数のminterm数を計算する
  ymuint n0 = mterm_step(vp->edge0(pol), mc_map);
  ymuint n1 = mterm_step(vp->edge1(pol), mc_map);

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  ymuint ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    mc_map[e] = ans;
  }

  return ans;
}

// Walsh変換を行なう．ただし，0次の係数しか求めない．
// n は全入力数
mpz_class
BddMgrClassic::walsh0(BddEdge e,
		      ymuint n)
{
  if ( e.is_overflow() ) {
    return 0;
  }
  if ( e.is_error() ) {
    return 0;
  }

  ymuint bitsize = n + 2;
  if ( bitsize < sizeof(int) * 8 ) {
    // 値の範囲が int に収まるのなら int 版の関数を呼ぶ．
    n_invect_int = 1U << n;

    hash_map<Node*, ymint> result_map;
    ymuint result = wt0_step(e, result_map);

    return mpz_class(result);
  }
  else {
    n_invect = mpz_class(1U) << n;

    hash_map<Node*, mpz_class> result_map;
    mpz_class result = wt0_step(e, result_map);

    return result;
  }
}

// Walsh spectrumの0次の係数を求める処理
mpz_class
BddMgrClassic::wt0_step(BddEdge e,
			hash_map<Node*, mpz_class>& result_map)
{
  if ( e.is_zero() ) {
    return n_invect;
  }
  if ( e.is_one() ) {
    return -n_invect;
  }

  // まずハッシュ表を探す．
  Node* vp = get_node(e);
  tPol pol = e.pol();
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, mpz_class>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  // 子供の値を再帰的に計算してそれを足し合わせる．
  mpz_class n0 = wt0_step(vp->edge0(), result_map);
  mpz_class n1 = wt0_step(vp->edge1(), result_map);
  mpz_class result = (n0 + n1) >> 1;

  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// Walsh spectrumの0次の係数を求める処理
// こちらは int 版
ymint
BddMgrClassic::wt0_step(BddEdge e,
			hash_map<Node*, ymint>& result_map)
{
  if ( e.is_zero() ) {
    return static_cast<ymint>(n_invect_int);
  }
  if ( e.is_one() ) {
    return -static_cast<ymint>(n_invect_int);
  }

  // まずハッシュ表を探す．
  Node* vp = get_node(e);
  tPol pol = e.pol();
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, ymint>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  // 子供の値を再帰的に計算してそれを足し合わせる．
  ymint n0 = wt0_step(vp->edge0(), result_map);
  ymint n1 = wt0_step(vp->edge1(), result_map);
  ymint result = (n0 + n1) >> 1;

  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// Walsh変換を行なう．ただし，1次の係数しか求めない．
// n は全入力数
mpz_class
BddMgrClassic::walsh1(BddEdge e,
		      VarId var,
		      ymuint n)
{
  if ( e.is_invalid() ) {
    return 0;
  }

  w_level = level(var);

  // var でコファクターを取るので必要なビット数をひとつ減らす
  ymuint bitsize = n + 1;
  if ( bitsize < sizeof(ymint) * 8 ) {
    // 値の範囲が int に収まるのなら int 版の関数を呼ぶ．
    n_invect_int = 1U << n;

    hash_map<Node*, ymint> result_map;
    ymint result = wt1_step(e, result_map);

    return mpz_class(result);
  }
  else {
    n_invect = mpz_class(1U) << n;

    hash_map<Node*, mpz_class> result_map;
    mpz_class result = wt1_step(e, result_map);

    return result;
  }
}

// Walsh spectrumの1次の係数を求める処理
mpz_class
BddMgrClassic::wt1_step(BddEdge e,
			hash_map<Node*, mpz_class>& result_map)
{
  if ( e.is_const() ) {
    return 0;
  }

  Node* vp = get_node(e);
  ymuint level = vp->level();
  if ( level > w_level ) {
    return 0;
  }

  // まずハッシュ表を探す．
  tPol pol = e.pol();
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, mpz_class>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  mpz_class result;
  if ( level < w_level ) {
    // 子供の値を再帰的に計算してそれを足し合わせる．
    mpz_class n0 = wt1_step(vp->edge0(), result_map);
    mpz_class n1 = wt1_step(vp->edge1(), result_map);
    result = (n0 + n1) >> 1;
  }
  else {
    // 正のコファクターの結果から負のコファクターの結果を引く
    mpz_class n0 = wt0_step(vp->edge0(), result_map);
    mpz_class n1 = wt0_step(vp->edge1(), result_map);
    result = n0 - n1;
  }
  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// Walsh spectrumの1次の係数を求める処理
// int 版
ymint
BddMgrClassic::wt1_step(BddEdge e,
			hash_map<Node*, ymint>& result_map)
{
  if ( e.is_const() ) {
    return 0;
  }

  Node* vp = get_node(e);
  ymuint level = vp->level();
  if ( level > w_level ) {
    return 0;
  }

  // まずハッシュ表を探す．
  tPol pol = e.pol();
  ymuint ref = vp->refcount();
  if ( ref != 1 ) {
    hash_map<Node*, int>::iterator p = result_map.find(vp);
    if ( p != result_map.end() ) {
      if ( pol == kPolPosi ) {
	return p->second;
      }
      else {
	return -p->second;
      }
    }
  }

  // 未登録の場合．
  ymint result;
  if ( level < w_level ) {
    // 子供の値を再帰的に計算してそれを足し合わせる．
    ymint n0 = wt1_step(vp->edge0(), result_map);
    ymint n1 = wt1_step(vp->edge1(), result_map);
    result = (n0 + n1) >> 1;
  }
  else {
    // 正のコファクターの結果から負のコファクターの結果を引く
    ymint n0 = wt0_step(vp->edge0(), result_map);
    ymint n1 = wt0_step(vp->edge1(), result_map);
    result = n0 - n1;
  }
  if ( ref != 1 ) {
    result_map[vp] = result;
  }

  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

END_NAMESPACE_YM_BDD
