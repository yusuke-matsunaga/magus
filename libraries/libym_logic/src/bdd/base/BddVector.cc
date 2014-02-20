
/// @file BddVector.cc
/// @brief BddVector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/BddVector.h"
#include "logic/Bdd.h"
#include "logic/BddMgr.h"
#include "logic/BddVarSet.h"

#include "utils/HeapTree.h"

#include "BddMgrImpl.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// 入力数がこの値よりも小さかったらヒープ木は使わない．
const ymuint kNiLimit = 8;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス BddVector
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @param[in] mgr BddMgr
BddVector::BddVector(BddMgr& mgr) :
  mMgr(mgr)
{
}

// @brief 要素数を指定したコンストラクタ
// @param[in] mgr BddMgr
// @param[in] n 要素数
BddVector::BddVector(BddMgr& mgr,
		     ymuint n) :
  vector<Bdd>(n),
  mMgr(mgr)
{
  assert_cond( size() == n, __FILE__, __LINE__);
}

// @brief デストラクタ
BddVector::~BddVector()
{
}

// 要素のBDDの論理積を求める．
Bdd
BddVector::and_op() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_one();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 & bdd1;
  }
  if ( n == 3 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 & bdd1 & bdd2;
  }
  if ( n == 4 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    ++ p;
    Bdd bdd3 = *p;
    return bdd0 & bdd1 & bdd2 & bdd3;
  }

  Bdd ans;
  if ( n < kNiLimit ) {
    BddVector::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans &= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddVector::const_iterator p = begin();
	 p != end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, bdd.node_count());
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans &= work.getmin();
      ans = work.xchgmin(ans, ans.node_count());
    }
  }
  return ans;
}

// 複数のBDDの論理和を求める．
Bdd
BddVector::or_op() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_zero();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 | bdd1;
  }
  if ( n == 3 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 | bdd1 | bdd2;
  }
  if ( n == 4 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    ++ p;
    Bdd bdd3 = *p;
    return bdd0 | bdd1 | bdd2 | bdd3;
  }

  Bdd ans;
  if ( n < kNiLimit ) {
    BddVector::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans |= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddVector::const_iterator p = begin();
	 p != end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, bdd.node_count());
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans |= work.getmin();
      ans = work.xchgmin(ans, ans.node_count());
    }
  }
  return ans;
}

// 複数のBDDの排他的論理和を求める．
Bdd
BddVector::xor_op() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_zero();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 ^ bdd1;
  }
  if ( n == 3 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 ^ bdd1 ^ bdd2;
  }
  if ( n == 4 ) {
    BddVector::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    ++ p;
    Bdd bdd3 = *p;
    return bdd0 ^ bdd1 ^ bdd2 ^ bdd3;
  }

  Bdd ans;
  if ( n < kNiLimit ) {
    BddVector::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans ^= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddVector::const_iterator p = begin();
	 p != end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, bdd.node_count());
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans ^= work.getmin();
      ans = work.xchgmin(ans, ans.node_count());
    }
  }
  return ans;
}

// @brief BDD ベクタが使っているノード数を数える．
// @param[in] array BDD ベクタ
// @return BDD ベクタが使っているノード数
ymuint64
BddVector::node_count() const
{
  if ( empty() ) {
    return 0;
  }
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  return mgr->node_count(edge_list);
}

// @brief BDD ベクタのサポート変数集合の計算 (VarVector)
// @param[in] bdd_array BDD のベクタ
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
ymuint
BddVector::support(VarVector& sup) const
{
  if ( empty() ) {
    sup.clear();
    return 0;
  }
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  mgr->mark_support(edge_list);
  return mgr->mark_to_vector(sup);
}

// @brief BDD ベクタのサポート変数集合の計算 (VarList)
// @param[in] bdd_array BDD のベクタ
// @param[in] sup サポート変数集合を格納するリスト
// @return サポート変数集合の要素数
ymuint
BddVector::support(VarList& sup) const
{
  if ( empty() ) {
    sup.clear();
    return 0;
  }
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief BDD ベクタのサポート変数集合の計算 (BddVarSet)
// @param[in] bdd_array BDD のベクタ
// @return サポート変数集合
BddVarSet
BddVector::support() const
{
  if ( empty() ) {
    return BddVarSet(mMgr);
  }
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  mgr->mark_support(edge_list);
  BddEdge ans = mgr->mark_to_bdd();
  return BddVarSet(Bdd(mgr, ans));
}

// @brief BDD ベクタのサポート変数集合の要素数の計算
// @param[in] bdd_array BDD のベクタ
// @return サポート変数集合の要素数
ymuint
BddVector::support_size() const
{
  if ( empty() ) {
    return 0;
  }
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  // 今は手抜きで bdd_array 中の BDD のマネージャは全部同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  return mgr->mark_support(edge_list);
}

END_NAMESPACE_YM_BDD
