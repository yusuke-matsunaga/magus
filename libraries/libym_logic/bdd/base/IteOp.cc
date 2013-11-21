
/// @file IteOp.cc
/// @brief IteOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "IteOp.h"
#include "BddBinOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス IteOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
// @param[in] and_op AND 演算オブジェクト
// @param[in] xor_op XOR 演算オブジェクト
IteOp::IteOp(BddMgrImpl* mgr,
	     BddBinOp* and_op,
	     BddBinOp* xor_op) :
  BddTriOp(mgr, "ite_op"),
  mAndOp(and_op),
  mXorOp(xor_op)
{
}

// @brief デストラクタ
IteOp::~IteOp()
{
}

// @brief 演算を行う関数
// @param[in] e1, e2, e3 オペランド
// @return 演算結果を返す．
BddEdge
IteOp::apply(BddEdge e1,
	     BddEdge e2,
	     BddEdge e3)
{
  // エラー状態のチェック
  if ( e1.is_error() || e2.is_error() || e3.is_error() ) {
    // どちらかがエラー
    return BddEdge::make_error();
  }
  if ( e1.is_overflow() || e2.is_overflow() || e3.is_overflow() ) {
    // どちらかがオーバーフロー
    return BddEdge::make_overflow();
  }

  return apply_step(e1, e2, e3);
}

// @brief 実際の演算を行う関数
BddEdge
IteOp::apply_step(BddEdge f,
		  BddEdge g,
		  BddEdge h)
{
  if ( f.is_one() ) {
    // f が 1 なら 答は常に g
    return g;
  }
  if ( f.is_zero() ) {
    // f が 0 なら 答えは常に h
    return h;
  }
  if ( g == h ) {
    // g と h が等しければ f に無関係に g (= h)
    return g;
  }
  if ( check_reverse(g, h) ) {
    // g と h が逆相で等しければ XOR(f, h)
    //   f g + ~f h で g = ~h なので
    // = f ~h + ~f h = XOR(f, h)
    return mXorOp->apply(f, h);
  }
  if ( g.is_one() || f == g ) {
    // g が 1 なら f + f ~h = f + h
    // f == g でも f + f ~h = f + h
    // 実は g >= f なら常にそうだが，上記以外は簡単にわからない．
    return ~mAndOp->apply(~f, ~h);
  }
  if ( g.is_zero() || check_reverse(f, g) ) {
    // g が 0 なら f ~ h
    // f == ~g でも f ~h
    // 実は g f == 0 なら常にそうだが，上記以外は簡単にわからない．
    return mAndOp->apply(~f, h);
  }
  if ( h.is_one() || check_reverse(f, h) ) {
    // h が 1 なら f g + ~f = ~f + g
    // f == ~h でも fg + ~f = ~f + g
    // 実は ~f <= h なら常にそうだが，上記以外は簡単にわからない．
    return ~mAndOp->apply(f, ~g);
  }
  if ( h.is_zero() || f == h ) {
    // h が 0 なら f g
    // f == h でも ~f h == 0 より f g
    // 実は ~f h == 0 なら常にそうだが，上記以外は簡単にわからない．
    return mAndOp->apply(f, g);
  }
  // この時点で f, g, h は終端ではない．

  // 演算結果テーブルが当たりやすくなるように順序を正規化する．
  if ( g > h ) {
    BddEdge tmp = g;
    g = h;
    h = tmp;
    // then と else を入れ替えたんだから条件も反転しなければならない．
    f = ~f;
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
  ymuint top_level = f_level;
  if ( top_level > g_level ) {
    top_level = g_level;
  }
  if ( top_level > h_level ) {
    top_level = h_level;
  }

  tPol f_pol = f.pol();
  BddEdge f_0, f_1;
  split1(top_level, f_level, f, f_vp, f_pol, f_0, f_1);

  tPol g_pol = g.pol();
  BddEdge g_0, g_1;
  split1(top_level, g_level, g, g_vp, g_pol, g_0, g_1);

  tPol h_pol = h.pol();
  BddEdge h_0, h_1;
  split1(top_level, h_level, h, h_vp, h_pol, h_0, h_1);

  BddEdge result;

  if ( f_0.is_zero() && f_1.is_one() ) {
    // f が肯定のリテラル関数でもっとも小さいレベルならそのままノードを作る．
    result = new_node(f_level, h_0, g_1);
  }
  else if ( f_0.is_one() && f_1.is_zero() ) {
    // f が否定のリテラル関数でもっとも小さいレベルならそのままノードを作る．
    result = new_node(f_level, g_0, h_1);
  }
  else {
    // 演算結果テーブルを調べる．
    result = get(f, g, h);
    if ( result.is_error() ) {
      BddEdge r_0 = apply_step(f_0, g_0, h_0);
      if ( r_0.is_overflow() ) {
	return BddEdge::make_overflow();
      }
      BddEdge r_1 = apply_step(f_1, g_1, h_1);
      if ( r_1.is_overflow() ) {
	return BddEdge::make_overflow();
      }
      result = new_node(top_level, r_0, r_1);
      put(f, g, h, result);
    }
  }
  return BddEdge(result, ans_pol);
}

END_NAMESPACE_YM_BDD

