
/// @file McOp.cc
/// @brief McOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "McOp.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス McOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
McOp::McOp(BddMgrImpl* mgr) :
  BddOp(mgr)
{
}

// @brief デストラクタ
McOp::~McOp()
{
}

// @brief 演算を行う関数
// @param[in] e 根の枝
// @param[in] nvar 変数の数
// @return e が表す BDD の最小項の数を返す．
mpz_class
McOp::apply(BddEdge e,
	    ymuint nvar)
{
  if ( e.is_invalid() ) {
    return 0;
  }

  ymuint bitsize = nvar + 1;
  if ( bitsize < sizeof(ymuint32) * 8 ) {
    // 値の範囲が ymuint32 に収まるのなら int 版の関数を呼ぶ．

    // 全入力ベクトルの数の計算
    mAllCount2 = 1UL << nvar;

    mCompTbl2.clear();
    ymuint32 ans = count_sub2(e);

    return mpz_class(ans);
  }
  else {
    // 全入力ベクトルの数の計算
    mAllCount1 = mpz_class(1U) << nvar;

    mCompTbl1.clear();
    mpz_class ans = count_sub1(e);

    return ans;
  }
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
McOp::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数(mpz_class 版)
mpz_class
McOp::count_sub1(BddEdge e)
{
  if ( e.is_one() ) {
    return mAllCount1;
  }
  if ( e.is_zero() ) {
    return mpz_class(0);
  }

  BddNode* node = e.get_node();
  tPol pol = e.pol();
  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    BddEdgeMpzMap::iterator p = mCompTbl1.find(e);
    if ( p != mCompTbl1.end() ) {
      return p->second;
    }
  }

  // 子ノードが表す関数のminterm数を計算する
  mpz_class n0 = count_sub1(node->edge0(pol));
  mpz_class n1 = count_sub1(node->edge1(pol));

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  mpz_class ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl1.insert(make_pair(e, ans));
  }

  return ans;
}

// @brief apply() の下請け関数(ymuint 版)
ymuint32
McOp::count_sub2(BddEdge e)
{
  if ( e.is_one() ) {
    return mAllCount2;
  }
  if ( e.is_zero() ) {
    return 0;
  }

  BddNode* node = e.get_node();
  tPol pol = e.pol();
  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    BddEdgeUint32Map::iterator p = mCompTbl2.find(e);
    if ( p != mCompTbl2.end() ) {
      return p->second;
    }
  }

  // 子ノードが表す関数のminterm数を計算する
  ymuint32 n0 = count_sub2(node->edge0(pol));
  ymuint32 n1 = count_sub2(node->edge1(pol));

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  ymuint32 ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl2.insert(make_pair(e, ans));
  }

  return ans;
}

END_NAMESPACE_YM_BDD
