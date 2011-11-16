
/// @file libym_logic/bdd/base/Dumper.cc
/// @brief Dumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dumper.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Dumper.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// IdMgr
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
ymuint
IdMgr::num() const
{
  return mNext;
}

// e がすでに ID 番号を持っていたら true を返す．
bool
IdMgr::has_id(BddEdge e) const
{
  BddEdge e_p(e);
  e_p.normalize();
  return mMap.count(e_p);
}

// e の ID 番号を返す．
// 未登録ならば新しい番号を割り振る．
ymuint
IdMgr::id(BddEdge e)
{
  BddEdge e_p(e);
  e_p.normalize();
  ymuint id;
  hash_map<BddEdge, ymuint>::iterator p = mMap.find(e_p);
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


//////////////////////////////////////////////////////////////////////
// Displayer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Displayer::Displayer(BddMgrImpl* mgr,
		     ostream& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Displayer::~Displayer()
{
}

// e の ID 番号を出力する．
void
Displayer::display_id(BddEdge e)
{
  ymuint id = mIdMgr.id(e);
  ios::fmtflags save = mStream.flags();
  mStream << setw(9) << id;
  mStream.flags(save);
}

// e の内容を出力する．
void
Displayer::display_name(BddEdge e)
{
  if ( e.is_zero() ) {
    mStream << "     val_0 ";
  }
  else if ( e.is_one() ) {
    mStream << "     val_1 ";
  }
  else if ( e.is_error() ) {
    mStream << "     error ";
  }
  else if ( e.is_overflow() ) {
    mStream << "  overflow ";
  }
  else {
    mStream << " ";
    display_id(e);
    tPol p = e.pol();
    mStream << ((p == kPolPosi) ? ' ' : '~');
  }
}

// display_root の下請関数
void
Displayer::display_step(BddEdge e)
{
  if ( e.is_leaf() ) {
    return;
  }
  BddEdge e_p(e);
  e_p.normalize();
  if ( mMark.count(e_p) ) {
    return;
  }
  mMark.insert(e_p);
  display_id(e_p);
  BddEdge e0;
  BddEdge e1;
  VarId id = mMgr->root_decomp(e_p, e0, e1);
  tLevel level = mMgr->level(id);
  ios::fmtflags save = mStream.flags();
  mStream << ":IDX=" << setw(3) << id
	  << ":LVL=" << setw(3) << level
	  << ": EDGE0=";
  mStream.flags(save);
  display_name(e0);
  mStream << ": EDGE1=";
  display_name(e1);
  mStream << endl;
  display_step(e0);
  display_step(e1);
}

// e を根とするBDDの内容を出力する．
void
Displayer::display_root(BddEdge e)
{
  display_name(e);
  mStream << "\tmgr: " << mMgr->name() << endl;
  display_step(e);
}

// 登録された節点数を返す．
ymuint
Displayer::num() const
{
  return mIdMgr.num();
}


//////////////////////////////////////////////////////////////////////
// Dumper
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

// @brief 一つのBDDをダンプする．
void
Dumper::write(BddEdge e)
{
  dump_node(e);
  // エンドマーク
  mStream << kVarIdIllegal;
  // 出力数
  mStream << static_cast<ymuint32>(1);
  dump_edge(e);
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
  BddEdge e0;
  BddEdge e1;
  VarId varid = mMgr->root_decomp(e_p, e0, e1);
  dump_node(e0);
  dump_node(e1);
  mIdMgr.id(e_p);
  mStream << varid;
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
    ymuint32 id = mIdMgr.id(e);
    mStream << id;
  }
}


//////////////////////////////////////////////////////////////////////
// Restorer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Restorer::Restorer(BddMgrImpl* mgr,
		   BinI& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Restorer::~Restorer()
{
}

// 読み込む．
// 読み込んだBDDの根の枝の数を返す．
// エラーが起きたら 0 を返す．
ymuint
Restorer::read()
{
  mRootVector.clear();
  mEdgeVector.clear();
  for ( ; ; ) {
    VarId varid;
    mStream >> varid;
    if ( varid == kVarIdIllegal ) {
      // エンドマーク
      break;
    }
    BddEdge e0 = read_edge();
    BddEdge e1 = read_edge();
    BddEdge ans = mMgr->make_bdd(varid, e0, e1);
    mEdgeVector.push_back(ans);
  }
  ymuint32 n;
  mStream >> n;
  for (ymuint i = 0; i < n; ++ i) {
    // 根のノードの記述
    BddEdge root = read_edge();
    mRootVector.push_back(root);
  }
  return mRootVector.size();
}

// pos 番目の枝を返す．
BddEdge
Restorer::root(ymuint pos)
{
  if ( pos < mRootVector.size() ) {
    return mRootVector[pos];
  }
  return BddEdge::make_error();
}

// 内部の枝を指す文字列から枝を得る．
// 見つからなければ kEdgeError を返す．
BddEdge
Restorer::read_edge()
{
  ymuint8 type;
  mStream >> type;
  bool negated = false;
  switch ( type ) {
  case 0: return BddEdge::make_zero();
  case 1: return BddEdge::make_one();
  case 2: return BddEdge::make_error();
  case 3: return BddEdge::make_overflow();
  case 4: break;
  case 5: negated = true; break;
  default: return BddEdge::make_error();
  }
  ymuint32 id;
  mStream >> id;
  if ( id < mEdgeVector.size() ) {
    BddEdge ans = mEdgeVector[id];
    if ( negated ) {
      ans.negate();
    }
    return ans;
  }
  return BddEdge::make_error();
}

END_NAMESPACE_YM_BDD
