
/// @file Dumper.cc
/// @brief Dumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Dumper.h"

#include "ym_logic/Bdd.h"
#include "ym_logic/BddVector.h"
#include "ym_logic/BddList.h"


BEGIN_NAMESPACE_YM_BDD

// @brief 内容のダンプ
void
Bdd::dump(BinO& s) const
{
  Dumper dumper(mMgr, s);
  BddEdge e(mRoot);
  dumper.write(vector<BddEdge>(1, e));
}

// @brief 内容をダンプする．
// @param[in] s 出力ストリーム
void
BddVector::dump(BinO& s) const
{
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  Dumper dumper(mgr, s);
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  dumper.write(edge_list);
}

// @brief 内容をダンプする．
// @param[in] s 出力ストリーム
void
BddList::dump(BinO& s) const
{
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  Dumper dumper(mgr, s);
  vector<BddEdge> edge_list;
  edge_list.reserve(size());
  for (BddList::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    edge_list.push_back(e);
  }
  dumper.write(edge_list);
}


//////////////////////////////////////////////////////////////////////
// クラス Dumper
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Dumper::Dumper(BddMgrImpl* mgr,
	       BinO& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Dumper::~Dumper()
{
}

// @brief 複数のBDDをダンプする．
void
Dumper::write(const vector<BddEdge>& edge_list)
{
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    dump_node(e);
  }
  // エンドマーク
  mStream << kVarIdIllegal;
  // 出力数
  mStream << static_cast<ymuint32>(edge_list.size());
  for (vector<BddEdge>::const_iterator p = edge_list.begin();
       p != edge_list.end(); ++ p) {
    BddEdge e = *p;
    dump_edge(e);
  }
}

// @brief e で指されたBDDノードの内容を出力する．
void
Dumper::dump_node(BddEdge e)
{
  if ( e.is_leaf() || mIdMgr.has_id(e) ) {
    return;
  }
  BddEdge e_p(e);
  e_p.normalize();
  BddNode* node = e_p.get_node();
  BddEdge e0 = node->edge0();
  BddEdge e1 = node->edge1();
  ymuint level = node->level();
  VarId id = mMgr->varid(level);
  dump_node(e0);
  dump_node(e1);
  mIdMgr.id(e_p);
  mStream << id;
  dump_edge(e0);
  dump_edge(e1);
}

// 枝の内容を書き出す．
void
Dumper::dump_edge(BddEdge e)
{
  ymuint8 type;
  if ( e.is_zero() ) {
    type = 0;
  }
  else if ( e.is_one() ) {
    type = 1;
  }
  else if ( e.is_error() ) {
    type = 2;
  }
  else if ( e.is_overflow() ) {
    type = 3;
  }
  else {
    type = 4;
    tPol p = e.pol();
    if ( p == kPolNega ) {
      type = 5;
    }
  }
  mStream << type;
  if ( type >= 4 ) {
    ymuint64 id = mIdMgr.id(e);
    mStream << id;
  }
}

END_NAMESPACE_YM_BDD
