
/// @file Printer.cc
/// @brief Printer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "Printer.h"
#include "logic/Zdd.h"
#include "logic/ZddVector.h"
#include "logic/ZddList.h"
#include "logic/ZddMgr.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief ZDD の内容を書き出す．
ymuint64
Zdd::print(ostream& s) const
{
  Printer printer(mMgr, s);
  printer.print_root(ZddEdge(mRoot));
  return printer.num();
}

// @brief ZDD ベクタの内容を書き出す
// @param[in] array ZDD ベクタ
// @param[in] s 出力ストリーム
ymuint64
ZddVector::print(ostream& s) const
{
  if ( empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = mMgr.mImpl;
  Printer printer(mgr, s);
  for (ZddVector::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    printer.print_root(ZddEdge(zdd.mRoot));
  }
  return printer.num();
}

// @brief ZDD リストの内容を書き出す
// @param[in] array ZDD リスト
// @param[in] s 出力ストリーム
ymuint64
ZddList::print(ostream& s) const
{
  if ( empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  ZddMgrImpl* mgr = mMgr.mImpl;
  Printer printer(mgr, s);
  for (ZddList::const_iterator p = begin();
       p != end(); ++ p) {
    Zdd zdd = *p;
    printer.print_root(ZddEdge(zdd.mRoot));
  }
  return printer.num();
}


//////////////////////////////////////////////////////////////////////
// クラス Printer
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
  ymuint64 id = mIdMgr.id(e);
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
    bool z = e.zattr();
    mStream << ((z) ? '*' : ' ');
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
  ZddNode* node = e_p.get_node();
  ymuint level = node->level();
  VarId id = mMgr->varid(level);
  ios::fmtflags save = mStream.flags();
  mStream << ":IDX=" << setw(3) << id
	  << ":LVL=" << setw(3) << level
	  << ": EDGE0=";
  mStream.flags(save);
  ZddEdge e0 = node->edge0();
  print_name(e0);
  mStream << ": EDGE1=";
  ZddEdge e1 = node->edge1();;
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
ymuint64
Printer::num() const
{
  return mIdMgr.num();
}

END_NAMESPACE_YM_ZDD
