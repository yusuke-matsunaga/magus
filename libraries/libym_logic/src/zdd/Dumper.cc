
/// @file Dumper.cc
/// @brief Dumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Dumper.h"
#include "YmLogic/Zdd.h"
#include "YmLogic/ZddVector.h"
#include "YmLogic/ZddList.h"
#include "YmLogic/ZddMgr.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief 内容のダンプ
void
Zdd::dump(ODO& s) const
{
  Dumper dumper(mMgr, s);
  ZddEdge e(mRoot);
  dumper.dump(e);
  dumper.dump_edge(e);
}

// @brief ZDD ベクタの内容をダンプする．
// @param[in] s 出力ストリーム
void
ZddVector::dump(ODO& s) const
{
  if ( empty() ) {
    return;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  Dumper dumper(mMgr.mImpl, s);
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump(ZddEdge(zdd.mRoot));
  }
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump_edge(ZddEdge(zdd.mRoot));
  }
}

// @brief ZDD リストの内容をダンプする．
// @param[in] s 出力ストリーム
void
ZddList::dump(ODO& s) const
{
  if ( empty() ) {
    return;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  Dumper dumper(mMgr.mImpl, s);
  for (ZddList::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump(ZddEdge(zdd.mRoot));
  }
  for (ZddList::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    dumper.dump_edge(ZddEdge(zdd.mRoot));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Dumper
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Dumper::Dumper(ZddMgrImpl* mgr,
	       ODO& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Dumper::~Dumper()
{
}

// e を根とするZDDの内容を出力する．
void
Dumper::dump(ZddEdge e)
{
  if ( e.is_leaf() || mIdMgr.has_id(e) ) {
    return;
  }
  ZddEdge e_p(e);
  e_p.normalize();
  ZddNode* node = e_p.get_node();
  ymuint level = node->level();
  ZddEdge e0 = node->edge0();
  dump(e0);
  ZddEdge e1 = node->edge1();
  dump(e1);
  mIdMgr.id(e_p);
  mStream << level << ":";
  dump_edge(e0);
  mStream << ":";
  dump_edge(e1);
}

// 枝の内容を書き出す．
void
Dumper::dump_edge(ZddEdge e)
{
#if 0
  if ( e.is_zero() ) {
    mStream << "F";
  }
  else if ( e.is_one() ) {
    mStream << "T";
  }
  else if ( e.is_error() ) {
    mStream << "E";
  }
  else if ( e.is_overflow() ) {
    mStream << "O";
  }
  else {
    tPol p = e.pol();
    mStream << ((p == kPolPosi) ? "P" : "N");
    ymuint id = mIdMgr.id(e);
    mStream << id;
  }
#endif
}

END_NAMESPACE_YM_ZDD
