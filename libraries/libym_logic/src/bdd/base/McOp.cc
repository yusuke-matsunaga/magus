
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
MpInt
McOp::apply(BddEdge e,
	    ymuint nvar)
{
  if ( e.is_invalid() ) {
    return MpInt(0);
  }

  ymuint bitsize = nvar + 1;
  if ( bitsize < sizeof(ymuint32) * 8 ) {
    // 値の範囲が ymuint32 に収まるのなら int 版の関数を呼ぶ．

    // 全入力ベクトルの数の計算
    mAllCount2 = 1UL << nvar;

    mCompTbl2.clear();
    ymuint32 ans = count_sub2(e);

    return MpInt(ans);
  }
  else {
    // 全入力ベクトルの数の計算
    mAllCount1 = MpInt(1U) << nvar;

    mCompTbl1.clear();
    MpInt ans = count_sub1(e);

    return ans;
  }
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
McOp::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数(MpInt 版)
MpInt
McOp::count_sub1(BddEdge e)
{
  if ( e.is_one() ) {
    return mAllCount1;
  }
  if ( e.is_zero() ) {
    return MpInt(0);
  }

  BddNode* node = e.get_node();
  bool inv = e.inv();
  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    MpInt ans;
    if ( mCompTbl1.find(e, ans) ) {
      return ans;
    }
  }

  // 子ノードが表す関数のminterm数を計算する
  MpInt n0 = count_sub1(node->edge0(inv));
  MpInt n1 = count_sub1(node->edge1(inv));

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  MpInt ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl1.add(e, ans);
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
  bool inv = e.inv();
  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    ymuint32 ans;
    if ( mCompTbl2.find(e, ans) ) {
      return ans;
    }
  }

  // 子ノードが表す関数のminterm数を計算する
  ymuint32 n0 = count_sub2(node->edge0(inv));
  ymuint32 n1 = count_sub2(node->edge1(inv));

  // 子ノードが表す関数の minterm 数を足して半分にしたものが
  // 親ノードが表す関数の minterm 数
  ymuint32 ans = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl2.add(e, ans);
  }

  return ans;
}

END_NAMESPACE_YM_BDD
