
/// @file IdMgr.cc
/// @brief IdMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IdMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// クラス IdMgr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
IdMgr::IdMgr()
{
  mNext = 0;
}

// デストラクタ
IdMgr::~IdMgr()
{
}

// 登録された節点数を返す．
ymuint64
IdMgr::num() const
{
  return mNext;
}

// e がすでに ID 番号を持っていたら true を返す．
bool
IdMgr::has_id(CNFddEdge e) const
{
  CNFddEdge e_p(e);
  e_p.normalize();
  return mMap.count(e_p);
}

// e の ID 番号を返す．
// 未登録ならば新しい番号を割り振る．
ymuint64
IdMgr::id(CNFddEdge e)
{
  CNFddEdge e_p(e);
  e_p.normalize();
  ymuint64 id;
  hash_map<CNFddEdge, ymuint64>::iterator p = mMap.find(e_p);
  if ( p == mMap.end() ) {
    id = mNext;
    ++ mNext;
    mMap.insert(make_pair(e_p, id));
  }
  else {
    id = p->second;
  }
  return id;
}

// クリアする．
void
IdMgr::clear()
{
  mMap.clear();
  mNext = 0;
}

END_NAMESPACE_YM_CNFDD
