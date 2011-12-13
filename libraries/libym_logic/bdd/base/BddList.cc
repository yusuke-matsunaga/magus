
/// @file BddList.cc
/// @brief BddList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/BddList.h"
#include "ym_logic/Bdd.h"
#include "ym_logic/BddMgr.h"

#include "ym_utils/HeapTree.h"


BEGIN_NAMESPACE_YM_BDD

BEGIN_NONAMESPACE

// 入力数がこの値よりも小さかったらヒープ木は使わない．
const ymuint kNiLimit = 8;

END_NONAMESPACE
#if 1
//////////////////////////////////////////////////////////////////////
// クラス BddList
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
// @param[in] mgr BddMgr
BddList::BddList(BddMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
BddList::~BddList()
{
}

// 要素のBDDの論理積を求める．
Bdd
BddList::and_op() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_one();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    BddList::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 & bdd1;
  }
  if ( n == 3 ) {
    BddList::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 & bdd1 & bdd2;
  }
  if ( n == 4 ) {
    BddList::const_iterator p = begin();
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
    BddList::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans &= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddList::const_iterator p = begin();
	 p != end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, nsYm::nsBdd::size(bdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans &= work.getmin();
      ans = work.xchgmin(ans, nsYm::nsBdd::size(ans));
    }
  }
  return ans;
}

// 複数のBDDの論理和を求める．
Bdd
BddList::or_op() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_zero();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    BddList::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 | bdd1;
  }
  if ( n == 3 ) {
    BddList::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 | bdd1 | bdd2;
  }
  if ( n == 4 ) {
    BddList::const_iterator p = begin();
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
    BddList::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans |= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddList::const_iterator p = begin();
	 p != end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, nsYm::nsBdd::size(bdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans |= work.getmin();
      ans = work.xchgmin(ans, nsYm::nsBdd::size(ans));
    }
  }
  return ans;
}

// 複数のBDDの排他的論理和を求める．
Bdd
BddList::xor_op() const
{
  ymuint n = size();
  if ( n == 0 ) {
    return mMgr.make_zero();
  }
  if ( n == 1 ) {
    return front();
  }
  if ( n == 2 ) {
    BddList::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 ^ bdd1;
  }
  if ( n == 3 ) {
    BddList::const_iterator p = begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 ^ bdd1 ^ bdd2;
  }
  if ( n == 4 ) {
    BddList::const_iterator p = begin();
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
    BddList::const_iterator p = begin();
    ans = *p;
    for (++ p; p != end(); ++ p) {
      ans ^= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddList::const_iterator p = begin();
	 p != end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, nsYm::nsBdd::size(bdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans ^= work.getmin();
      ans = work.xchgmin(ans, nsYm::nsBdd::size(ans));
    }
  }
  return ans;
}
#else

// 要素のBDDの論理積を求める．
Bdd
BddMgr::and_op(const BddList& bdds)
{
  ymuint n = bdds.size();
  if ( n == 0 ) {
    return make_one();
  }
  if ( n == 1 ) {
    return bdds.front();
  }
  if ( n == 2 ) {
    BddList::const_iterator p = bdds.begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 & bdd1;
  }
  if ( n == 3 ) {
    BddList::const_iterator p = bdds.begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 & bdd1 & bdd2;
  }
  if ( n == 4 ) {
    BddList::const_iterator p = bdds.begin();
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
    BddList::const_iterator p = bdds.begin();
    ans = *p;
    for (++ p; p != bdds.end(); ++ p) {
      ans &= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddList::const_iterator p = bdds.begin();
	 p != bdds.end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, size(bdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans &= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

// 複数のBDDの論理和を求める．
Bdd
BddMgr::or_op(const BddList& bdds)
{
  ymuint n = bdds.size();
  if ( n == 0 ) {
    return make_zero();
  }
  if ( n == 1 ) {
    return bdds.front();
  }
  if ( n == 2 ) {
    BddList::const_iterator p = bdds.begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 | bdd1;
  }
  if ( n == 2 ) {
    BddList::const_iterator p = bdds.begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 | bdd1 | bdd2;
  }
  if ( n == 3 ) {
    BddList::const_iterator p = bdds.begin();
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
    BddList::const_iterator p = bdds.begin();
    ans = *p;
    for (++ p; p != bdds.end(); ++ p) {
      ans |= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddList::const_iterator p = bdds.begin();
	 p != bdds.end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, size(bdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans |= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}

// 複数のBDDの排他的論理和を求める．
Bdd
BddMgr::xor_op(const BddList& bdds)
{
  ymuint n = bdds.size();
  if ( n == 0 ) {
    return make_zero();
  }
  if ( n == 1 ) {
    return bdds.front();
  }
  if ( n == 2 ) {
    BddList::const_iterator p = bdds.begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    return bdd0 ^ bdd1;
  }
  if ( n == 3 ) {
    BddList::const_iterator p = bdds.begin();
    Bdd bdd0 = *p;
    ++ p;
    Bdd bdd1 = *p;
    ++ p;
    Bdd bdd2 = *p;
    return bdd0 ^ bdd1 ^ bdd2;
  }
  if ( n == 4 ) {
    BddList::const_iterator p = bdds.begin();
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
    BddList::const_iterator p = bdds.begin();
    ans = *p;
    for (++ p; p != bdds.end(); ++ p) {
      ans ^= *p;
    }
  }
  else {
    SimpleHeapTree<Bdd> work;
    for (BddList::const_iterator p = bdds.begin();
	 p != bdds.end(); ++p) {
      Bdd bdd = *p;
      work.put(bdd, size(bdd));
    }
    ans = work.getmin();
    work.popmin();
    while ( !work.empty() ) {
      ans ^= work.getmin();
      ans = work.xchgmin(ans, size(ans));
    }
  }
  return ans;
}
#endif
END_NAMESPACE_YM_BDD
