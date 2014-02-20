
/// @file Dumper.cc
/// @brief Dumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Dumper.h"
#include "logic/Zdd.h"
//#include "logic/ZddVector.h"
//#include "logic/ZddList.h"
#include "logic/ZddMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

// @brief 内容のダンプ
void
CNFdd::dump(ODO& s) const
{
  Dumper dumper(mMgr, s);
  CNFddEdge e(mRoot);
  dumper.dump(e);
  dumper.dump_edge(e);
}

#if 0
// @brief ZDD ベクタの内容をダンプする．
// @param[in] s 出力ストリーム
void
CNFddVector::dump(ODO& s) const
{
  if ( empty() ) {
    return;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  Dumper dumper(mMgr.mImpl, s);
  for (CNFddVector::const_iterator p = begin();
       p != end(); ++ p) {
    CNFdd zdd = *p;
    dumper.dump(CNFddEdge(zdd.mRoot));
  }
  for (CNFddVector::const_iterator p = begin();
       p != end(); ++ p) {
    CNFdd zdd = *p;
    dumper.dump_edge(CNFddEdge(zdd.mRoot));
  }
}

// @brief ZDD リストの内容をダンプする．
// @param[in] s 出力ストリーム
void
CNFddList::dump(ODO& s) const
{
  if ( empty() ) {
    return;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  Dumper dumper(mMgr.mImpl, s);
  for (CNFddList::const_iterator p = begin();
       p != end(); ++ p) {
    CNFdd zdd = *p;
    dumper.dump(CNFddEdge(zdd.mRoot));
  }
  for (CNFddList::const_iterator p = begin();
       p != end(); ++ p) {
    CNFdd zdd = *p;
    dumper.dump_edge(CNFddEdge(zdd.mRoot));
  }
}
#endif


//////////////////////////////////////////////////////////////////////
// クラス Dumper
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Dumper::Dumper(CNFddMgrImpl* mgr,
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
Dumper::dump(CNFddEdge e)
{
  if ( e.is_leaf() || mIdMgr.has_id(e) ) {
    return;
  }
  CNFddEdge e2(e);
  e2.normalize();
  CNFddNode* node = e2.get_node();
  ymuint level = node->level();
  CNFddEdge e_0 = node->edge_0();
  dump(e_0);
  CNFddEdge e_p = node->edge_p();
  dump(e_p);
  CNFddEdge e_n = node->edge_n();
  dump(e_n);
  mIdMgr.id(e2);
  mStream << level << ":";
  dump_edge(e_0);
  mStream << ":";
  dump_edge(e_p);
  mStream << ":";
  dump_edge(e_n);
}

// 枝の内容を書き出す．
void
Dumper::dump_edge(CNFddEdge e)
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

END_NAMESPACE_YM_CNFDD
