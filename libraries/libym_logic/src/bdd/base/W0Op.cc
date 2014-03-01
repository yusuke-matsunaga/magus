
/// @file W0Op.cc
/// @brief W0Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "W0Op.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス W0Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
W0Op::W0Op(BddMgrImpl* mgr) :
  BddOp(mgr)
{
}

// @brief デストラクタ
W0Op::~W0Op()
{
}

// @brief 演算を行う関数
// @param[in] e 根の枝
// @param[in] nvar 変数の数
// @return e が表す BDD の最小項の数を返す．
mpz_class
W0Op::apply(BddEdge e,
	    ymuint nvar)
{
  if ( e.is_invalid() ) {
    return 0;
  }

  ymuint bitsize = nvar + 2;
  if ( bitsize < sizeof(ymint32) * 8 ) {
    // 値の範囲が ymint32 に収まるのなら int 版の関数を呼ぶ．

    // 全入力ベクトルの数の計算
    mAllCount2 = 1L << nvar;

    mCompTbl2.clear();
    ymint32 ans = count_sub2(e);

    return mpz_class(ans);
  }
  else {
    // 全入力ベクトルの数の計算
    mAllCount1 = mpz_class(1) << nvar;

    mCompTbl1.clear();
    mpz_class ans = count_sub1(e);

    return ans;
  }
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
W0Op::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数(mpz_class 版)
mpz_class
W0Op::count_sub1(BddEdge e)
{
  if ( e.is_zero() ) {
    return mAllCount1;
  }
  if ( e.is_one() ) {
    return -mAllCount1;
  }

  BddNode* node = e.get_node();
  bool inv = e.inv();
  // 極性違いは符号反転で表せるので正規化する．
  e.normalize();

  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    BddEdgeMpzMap::iterator p = mCompTbl1.find(e);
    if ( p != mCompTbl1.end() ) {
      mpz_class ans = p->second;
      if ( inv ) {
	ans = -ans;
      }
      return ans;
    }
  }

  // 子ノードが表す関数の walsh0 を計算する
  mpz_class n0 = count_sub1(node->edge0());
  mpz_class n1 = count_sub1(node->edge1());

  // 子ノードが表す関数の walsh0 を足して半分にしたものが
  // 親ノードが表す関数の walsh0
  mpz_class ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl1.insert(make_pair(e, ans));
  }

  // 極性を考慮して補正する．
  if ( inv ) {
    ans = -ans;
  }

  return ans;
}

// @brief apply() の下請け関数(ymint32 版)
ymint32
W0Op::count_sub2(BddEdge e)
{
  if ( e.is_zero() ) {
    return mAllCount2;
  }
  if ( e.is_one() ) {
    return -mAllCount2;
  }

  BddNode* node = e.get_node();
  bool inv = e.inv();

  // 極性違いは符号反転で表せるので正規化する．
  e.normalize();

  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    BddEdgeIntMap::iterator p = mCompTbl2.find(e);
    if ( p != mCompTbl2.end() ) {
      ymint32 ans = p->second;
      if ( inv ) {
	ans = -ans;
      }
      return ans;
    }
  }

  // 子ノードが表す関数の walsh0 を計算する
  ymint32 n0 = count_sub2(node->edge0());
  ymint32 n1 = count_sub2(node->edge1());

  // 子ノードが表す関数の walsh0 を足して半分にしたものが
  // 親ノードが表す関数の walsh0
  ymint32 ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl2.insert(make_pair(e, ans));
  }

  // 極性を考慮して補正する．
  if ( inv ) {
    ans = -ans;
  }

  return ans;
}

END_NAMESPACE_YM_BDD
