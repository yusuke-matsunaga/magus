
/// @file Dumper.cc
/// @brief Dumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Dumper.cc 700 2007-05-31 00:41:30Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Dumper.h"


BEGIN_NAMESPACE_YM_ZDD

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
IdMgr::has_id(ZddEdge e) const
{
  ZddEdge e_p(e);
  e_p.normalize();
  return mMap.count(e_p);
}

// e の ID 番号を返す．
// 未登録ならば新しい番号を割り振る．
ymuint
IdMgr::id(ZddEdge e)
{
  ZddEdge e_p(e);
  e_p.normalize();
  ymuint id;
  hash_map<ZddEdge, ymuint>::iterator p = mMap.find(e_p);
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
// Printer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Printer::Printer(ZddMgrImpl* mgr,
		 ostream& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Printer::~Printer()
{
}

// e の ID 番号を出力する．
void
Printer::print_id(ZddEdge e)
{
  ymuint id = mIdMgr.id(e);
  ios::fmtflags save = mStream.flags();
  mStream << setw(9) << id;
  mStream.flags(save);
}

// e の内容を出力する．
void
Printer::print_name(ZddEdge e)
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
    print_id(e);
    tPol p = e.pol();
    mStream << ((p == kPolPosi) ? ' ' : '~');
  }
}

// print_root の下請関数
void
Printer::print_step(ZddEdge e)
{
  if ( e.is_leaf() ) {
    return;
  }
  ZddEdge e_p(e);
  e_p.normalize();
  if ( mMark.count(e_p) ) {
    return;
  }
  mMark.insert(e_p);
  print_id(e_p);
  ZddEdge e0;
  ZddEdge e1;
  tVarId id = mMgr->root_decomp(e_p, e0, e1);
  tLevel level = mMgr->level(id);
  ios::fmtflags save = mStream.flags();
  mStream << ":IDX=" << setw(3) << id
	  << ":LVL=" << setw(3) << level
	  << ": EDGE0=";
  mStream.flags(save);
  print_name(e0);
  mStream << ": EDGE1=";
  print_name(e1);
  mStream << endl;
  print_step(e0);
  print_step(e1);
}

// e を根とするZDDの内容を出力する．
void
Printer::print_root(ZddEdge e)
{
  print_name(e);
  mStream << "\tmgr: " << mMgr->name() << endl;
  print_step(e);
}

// 登録された節点数を返す．
ymuint
Printer::num() const
{
  return mIdMgr.num();
}


//////////////////////////////////////////////////////////////////////
// Dumper
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Dumper::Dumper(ZddMgrImpl* mgr,
	       BinO& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Dumper::~Dumper()
{
  mStream.flush();
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
  ZddEdge e0;
  ZddEdge e1;
  tVarId varid = mMgr->root_decomp(e_p, e0, e1);
  dump(e0);
  dump(e1);
  mIdMgr.id(e_p);
  mStream << varid << ":";
  dump_edge(e0);
  mStream << ":";
  dump_edge(e1);
  mStream << endl;
}

// 枝の内容を書き出す．
void
Dumper::dump_edge(ZddEdge e)
{
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
}


//////////////////////////////////////////////////////////////////////
// Restorer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Restorer::Restorer(ZddMgrImpl* mgr,
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
// 読み込んだZDDの根の枝の数を返す．
// エラーが起きたら 0 を返す．
ymuint
Restorer::read()
{
  mRootVector.clear();
  mEdgeVector.clear();
  char buff[1024];
  while ( mStream.getline(buff, sizeof(buff), '\n') ) {
    if ( '0' <= buff[0] && buff[0] <= '9' ) {
      // 内部のノードの記述
      char* p = strtok(buff, ": \t");
      ymuint vid = atoi(p);
      p = strtok(NULL, ": \t");
      ZddEdge e0 = find_edge(p);
      p = strtok(NULL, ": \t");
      ZddEdge e1 = find_edge(p);
      ZddEdge ans = mMgr->make_zdd(vid, e0, e1);
      mEdgeVector.push_back(ans);
    }
    else {
      // 根のノードの記述
      mRootVector.push_back(find_edge(buff));
    }
  }
  return mRootVector.size();
}

// pos 番目の枝を返す．
ZddEdge
Restorer::root(ymuint pos)
{
  if ( pos < mRootVector.size() ) {
    return mRootVector[pos];
  }
  return ZddEdge::make_error();
}

// 内部の枝を指す文字列から枝を得る．
// 見つからなければ kEdgeError を返す．
ZddEdge
Restorer::find_edge(const char* str) const
{
  bool negated = false;
  switch ( str[0] ) {
  case 'T': return ZddEdge::make_one();
  case 'F': return ZddEdge::make_zero();
  case 'E': return ZddEdge::make_error();
  case 'O': return ZddEdge::make_overflow();
  case 'P': break;
  case 'N': negated = true; break;
  default: return ZddEdge::make_error();
  }
  ymuint pos = atoi(str + 1);
  if ( pos < mEdgeVector.size() ) {
    ZddEdge ans = mEdgeVector[pos];
    if ( negated ) {
      ans.negate();
    }
    return ans;
  }
  return ZddEdge::make_error();
}

END_NAMESPACE_YM_ZDD
