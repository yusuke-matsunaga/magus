
/// @file Printer.cc
/// @brief Printer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Printer.h"

#include "ym_logic/Bdd.h"
#include "ym_logic/BddVector.h"
#include "ym_logic/BddList.h"


BEGIN_NAMESPACE_YM_BDD

// @brief BDD の内容を書き出す．
ymuint64
Bdd::print(ostream& s) const
{
  BddEdge e(mRoot);
  Printer printer(mMgr, s);
  printer.print_root(e);
  return printer.num();
}

// @brief BDD ベクタの内容を書き出す
// @param[in] array BDD ベクタ
// @param[in] s 出力ストリーム
ymuint64
BddVector::print(ostream& s) const
{
  if ( empty() ) {
    return 0;
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  Printer printer(mgr, s);
  for (BddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    printer.print_root(e);
  }
  return printer.num();
}

// @brief BDD リストの内容を書き出す
// @param[in] array BDD リスト
// @param[in] s 出力ストリーム
ymuint64
BddList::print(ostream& s) const
{
  if ( empty() ) {
    return 0;
  }
  // 今は array の中のBDDのマネージャがすべて同じと仮定している．
  BddMgrImpl* mgr = mMgr.mImpl;
  Printer printer(mgr, s);
  for (BddList::const_iterator p = begin();
       p != end(); ++ p) {
    Bdd bdd = *p;
    BddEdge e(bdd.mRoot);
    printer.print_root(e);
  }
  return printer.num();
}


//////////////////////////////////////////////////////////////////////
// クラス Printer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Printer::Printer(BddMgrImpl* mgr,
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
Printer::print_id(BddEdge e)
{
  ymuint64 id = mIdMgr.id(e);
  ios::fmtflags save = mStream.flags();
  mStream << setw(9) << id;
  mStream.flags(save);
}

// e の内容を出力する．
void
Printer::print_name(BddEdge e)
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
Printer::print_step(BddEdge e)
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
  print_id(e_p);
  BddNode* node = e_p.get_node();
  BddEdge e0 = node->edge0();
  BddEdge e1 = node->edge1();
  ymuint level = node->level();
  VarId id = mMgr->varid(level);
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

// e を根とするBDDの内容を出力する．
void
Printer::print_root(BddEdge e)
{
  print_name(e);
  mStream << "\tmgr: " << mMgr->name() << endl;
  print_step(e);
}

// 登録された節点数を返す．
ymuint64
Printer::num() const
{
  return mIdMgr.num();
}

END_NAMESPACE_YM_BDD
