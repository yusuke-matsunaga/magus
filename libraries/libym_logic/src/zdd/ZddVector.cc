
/// @file ZddVector.cc
/// @brief ZddVector の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/ZddVector.h"
#include "YmLogic/Zdd.h"
#include "YmLogic/ZddMgr.h"
#include "ZddMgrImpl.h"
#include "YmUtils/HeapTree.h"


BEGIN_NAMESPACE_YM_ZDD

BEGIN_NONAMESPACE

// 入力数がこの値よりも小さかったらヒープ木は使わない．
const ymuint kNiLimit = 8;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス ZddVector
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @param[in] mgr ZddMgr
ZddVector::ZddVector(ZddMgr& mgr) :
  mMgr(mgr)
{
}

// @brief 要素数を指定したコンストラクタ
// @param[in] mgr ZddMgr
// @param[in] n 要素数
ZddVector::ZddVector(ZddMgr& mgr,
		     ymuint n) :
  vector<Zdd>(n),
  mMgr(mgr)
{
}

// @brief デストラクタ
ZddVector::~ZddVector()
{
}

// @brief intersection を求める．
Zdd
ZddVector::intersection() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_base();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    ZddVector::const_iterator p = begin();
    Zdd ans = *p;
    ++ p;
    ans &= *p;
    return ans;
  }
  if ( n == 3 ) {
    ZddVector::const_iterator p = begin();
    Zdd ans = *p;
    ++ p;
    ans &= *p;
    ++ p;
    ans &= *p;
    return ans;
  }
  if ( n == 4 ) {
    ZddVector::const_iterator p = begin();
    Zdd ans = *p;
    ++ p;
    ans &= *p;
    ++ p;
    ans &= *p;
    ++ p;
    ans &= *p;
    return ans;
  }

  Zdd ans;
  if ( n < kNiLimit ) {
    ZddVector::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans &= *p;
    }
  }
  else {
    SimpleHeapTree<Zdd> work;
    for (ZddVector::const_iterator p = begin();
	 p != end(); ++p) {
      Zdd zdd = *p;
      work.put(zdd, zdd.node_count());
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

// @brief union を求める．
Zdd
ZddVector::set_union() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_empty();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    ZddVector::const_iterator p = begin();
    Zdd ans = *p;
    ++ p;
    ans |= *p;
    return ans;
  }
  if ( n == 3 ) {
    ZddVector::const_iterator p = begin();
    Zdd ans = *p;
    ++ p;
    ans |= *p;
    ++ p;
    ans |= *p;
    return ans;
  }
  if ( n == 4 ) {
    ZddVector::const_iterator p = begin();
    Zdd ans = *p;
    ++ p;
    ans |= *p;
    ++ p;
    ans |= *p;
    ++ p;
    ans |= *p;
    return ans;
  }

  Zdd ans;
  if ( n < kNiLimit ) {
    ZddVector::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans |= *p;
    }
  }
  else {
    SimpleHeapTree<Zdd> work;
    for (ZddVector::const_iterator p = begin();
	 p != end(); ++p) {
      Zdd zdd = *p;
      work.put(zdd, zdd.node_count());
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

// @brief BDD のリストが使っているノード数を数える．
ymuint64
ZddVector::node_count() const
{
  if ( empty() ) {
    return 0;
  }
  vector<ZddEdge> edge_list;
  edge_list.reserve(size());
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = mMgr.mImpl;
  return mgr->node_count(edge_list);
}

// @brief ZDD ベクタのサポート変数集合の計算 (VarVector)
// @param[in] zdd_array ZDD のベクタ
// @param[in] sup サポート変数集合を格納するベクタ
// @return サポート変数集合の要素数
ymuint
ZddVector::support(VarVector& sup) const
{
  if ( empty() ) {
    sup.clear();
    return 0;
  }

  vector<ZddEdge> edge_list;
  edge_list.reserve(size());
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = mMgr.mImpl;
  mgr->mark_support(edge_list);
  return mgr->mark_to_vector(sup);
}

// @brief ZDD ベクタのサポート変数集合の計算 (VarList)
// @param[in] zdd_array ZDD のベクタ
// @param[in] sup サポート変数集合を格納するリスト
// @return サポート変数集合の要素数
ymuint
ZddVector::support(VarList& sup) const
{
  if ( empty() ) {
    sup.clear();
    return 0;
  }

  vector<ZddEdge> edge_list;
  edge_list.reserve(size());
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = mMgr.mImpl;
  mgr->mark_support(edge_list);
  return mgr->mark_to_list(sup);
}

// @brief ZDD ベクタのサポート変数集合の要素数の計算
// @param[in] zdd_array ZDD のベクタ
// @return サポート変数集合の要素数
ymuint
ZddVector::support_size() const
{
  if ( empty() ) {
    return 0;
  }

  vector<ZddEdge> edge_list;
  edge_list.reserve(size());
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    edge_list.push_back(ZddEdge(zdd.mRoot));
  }
  // 今は手抜きで zdd_array 中の ZDD のマネージャは全部同じと仮定している．
  ZddMgrImpl* mgr = mMgr.mImpl;
  return mgr->mark_support(edge_list);
}

END_NAMESPACE_YM_ZDD
