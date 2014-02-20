
/// @file W1Op.cc
/// @brief W1Op の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "W1Op.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// クラス W1Op
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] mgr マネージャ
W1Op::W1Op(BddMgrImpl* mgr) :
  BddOp(mgr)
{
}

// @brief デストラクタ
W1Op::~W1Op()
{
}

// @brief 演算を行う関数
// @param[in] e 根の枝
// @param[in] level 対象の変数のレベル
// @param[in] nvar 変数の数
// @return e が表す BDD の最小項の数を返す．
mpz_class
W1Op::apply(BddEdge e,
	    ymuint level,
	    ymuint nvar)
{
  if ( e.is_invalid() ) {
    return 0;
  }

  mLevel = level;

  ymuint bitsize = nvar + 2;
  if ( bitsize < sizeof(ymint32) * 8 ) {
    // 値の範囲が ymint32 に収まるのなら int 版の関数を呼ぶ．

    // 全入力ベクトルの数の計算
    mAllCount2 = 1L << nvar;

    mCompTbl2.clear();
    ymint32 ans = w1_sub2(e);

    return mpz_class(ans);
  }
  else {
    // 全入力ベクトルの数の計算
    mAllCount1 = mpz_class(1) << nvar;

    mCompTbl1.clear();
    mpz_class ans = w1_sub1(e);

    return ans;
  }
}

// @brief 次の GC で回収されるノードに関連した情報を削除する．
void
W1Op::sweep()
{
  // 何もしない．
}

// @brief apply() の下請け関数(mpz_class 版)
mpz_class
W1Op::w1_sub1(BddEdge e)
{
  if ( e.is_const() ) {
    return mpz_class(0);
  }

  BddNode* node = e.get_node();
  ymuint level = node->level();
  if ( level > mLevel ) {
    // mLevel の変数と無関係なので 0 を返す．
    return mpz_class(0);
  }

  tPol pol = e.pol();
  // 極性違いは符号反転で表せるので正規化する．
  e.normalize();

  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    hash_map<BddEdge, mpz_class>::iterator p = mCompTbl1.find(e);
    if ( p != mCompTbl1.end() ) {
      mpz_class ans = p->second;
      if ( pol == kPolNega ) {
	ans = -ans;
      }
      return ans;
    }
  }

  mpz_class result;
  if ( level < mLevel ) {
    // 子ノードが表す関数の walsh1 を計算する
    mpz_class n0 = w1_sub1(node->edge0());
    mpz_class n1 = w1_sub1(node->edge1());

    // 子ノードが表す関数の walsh1 を足して半分にしたものが
    // 親ノードが表す関数の walsh1
    result = (n0 + n1) >> 1;
  }
  else { // level == mLevel
    // 正のコファクターの結果から負のコファクターの結果を引く
    mpz_class n0 = w0_sub1(node->edge0());
    mpz_class n1 = w0_sub1(node->edge1());
    result = (n0 - n1) >> 1;
  }

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl1.insert(make_pair(e, result));
  }

  // 極性を考慮して補正する．
  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// @brief apply() の下請け関数(ymint32 版)
ymint32
W1Op::w1_sub2(BddEdge e)
{
  if ( e.is_const() ) {
    return 0;
  }

  BddNode* node = e.get_node();
  ymuint level = node->level();
  if ( level > mLevel ) {
    // mLevel の変数と無関係なので 0 を返す．
    return 0;
  }

  tPol pol = e.pol();
  // 極性違いは符号反転で表せるので正規化する．
  e.normalize();

  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    hash_map<BddEdge, ymint32>::iterator p = mCompTbl2.find(e);
    if ( p != mCompTbl2.end() ) {
      ymint32 ans = p->second;
      if ( pol == kPolNega ) {
	ans = -ans;
      }
      return ans;
    }
  }

  ymint32 result;
  if ( level < mLevel ) {
    // 子ノードが表す関数の walsh1 を計算する
    ymint32 n0 = w1_sub2(node->edge0());
    ymint32 n1 = w1_sub2(node->edge1());

    // 子ノードが表す関数の walsh1 を足して半分にしたものが
    // 親ノードが表す関数の walsh1
    result = (n0 + n1) >> 1;
  }
  else { // level == mLevel
    // 正のコファクターの結果から負のコファクターの結果を引く
    ymint32 n0 = w0_sub2(node->edge0());
    ymint32 n1 = w0_sub2(node->edge1());
    result = (n0 - n1) >> 1;
  }

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl2.insert(make_pair(e, result));
  }

  // 極性を考慮して補正する．
  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// @brief w1_sub() の下請け関数(mpz_class 版)
mpz_class
W1Op::w0_sub1(BddEdge e)
{
  if ( e.is_zero() ) {
    return mAllCount1;
  }
  if ( e.is_one() ) {
    return -mAllCount1;
  }

  BddNode* node = e.get_node();

  tPol pol = e.pol();
  // 極性違いは符号反転で表せるので正規化する．
  e.normalize();

  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    hash_map<BddEdge, mpz_class>::iterator p = mCompTbl1.find(e);
    if ( p != mCompTbl1.end() ) {
      mpz_class ans = p->second;
      if ( pol == kPolNega ) {
	ans = -ans;
      }
      return ans;
    }
  }

  // 子ノードが表す関数の walsh1 を計算する
  mpz_class n0 = w0_sub1(node->edge0());
  mpz_class n1 = w0_sub1(node->edge1());

  // 子ノードが表す関数の walsh1 を足して半分にしたものが
  // 親ノードが表す関数の walsh1
  mpz_class result = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl1.insert(make_pair(e, result));
  }

  // 極性を考慮して補正する．
  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

// @brief w1_sub() の下請け関数(ymint32 版)
ymint32
W1Op::w0_sub2(BddEdge e)
{
  if ( e.is_zero() ) {
    return mAllCount2;
  }
  if ( e.is_one() ) {
    return -mAllCount2;
  }

  BddNode* node = e.get_node();

  tPol pol = e.pol();
  // 極性違いは符号反転で表せるので正規化する．
  e.normalize();

  ymuint ref = node->refcount();
  if ( ref != 1 ) {
    // 複数回参照されていたらまず演算結果テーブルを探す．
    hash_map<BddEdge, ymint32>::iterator p = mCompTbl2.find(e);
    if ( p != mCompTbl2.end() ) {
      ymint32 ans = p->second;
      if ( pol == kPolNega ) {
	ans = -ans;
      }
      return ans;
    }
  }

  // 子ノードが表す関数の walsh1 を計算する
  ymint32 n0 = w0_sub2(node->edge0());
  ymint32 n1 = w0_sub2(node->edge1());

  // 子ノードが表す関数の walsh1 を足して半分にしたものが
  // 親ノードが表す関数の walsh1
  ymint32 result = (n0 + n1) >> 1;

  if ( ref != 1) {
    // 演算結果テーブルに答を登録する．
    mCompTbl2.insert(make_pair(e, result));
  }

  // 極性を考慮して補正する．
  if ( pol == kPolNega ) {
    result = -result;
  }

  return result;
}

END_NAMESPACE_YM_BDD
