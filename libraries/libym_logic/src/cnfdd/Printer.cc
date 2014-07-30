
/// @file Printer.cc
/// @brief Printer の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Printer.h"
#include "YmLogic/CNFdd.h"
//#include "YmLogic/CNFddVector.h"
//#include "YmLogic/CNFddList.h"
#include "YmLogic/CNFddMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

// @brief CNFDD の内容を書き出す．
ymuint64
CNFdd::print(ostream& s) const
{
  Printer printer(mMgr, s);
  printer.print_root(CNFddEdge(mRoot));
  return printer.num();
}

#if 0
// @brief ZDD ベクタの内容を書き出す
// @param[in] array ZDD ベクタ
// @param[in] s 出力ストリーム
ymuint64
CNFddVector::print(ostream& s) const
{
  if ( empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  CNFddMgrImpl* mgr = mMgr.mImpl;
  Printer printer(mgr, s);
  for (CNFddVector::const_iterator p = begin();
       p != end(); ++ p) {
    CNFdd zdd = *p;
    printer.print_root(CNFddEdge(zdd.mRoot));
  }
  return printer.num();
}

// @brief ZDD リストの内容を書き出す
// @param[in] array ZDD リスト
// @param[in] s 出力ストリーム
ymuint64
CNFddList::print(ostream& s) const
{
  if ( empty() ) {
    return 0;
  }
  // 今は array の中のZDDのマネージャがすべて同じと仮定している．
  CNFddMgrImpl* mgr = mMgr.mImpl;
  Printer printer(mgr, s);
  for (CNFddList::const_iterator p = begin();
       p != end(); ++ p) {
    CNFdd zdd = *p;
    printer.print_root(CNFddEdge(zdd.mRoot));
  }
  return printer.num();
}
#endif

//////////////////////////////////////////////////////////////////////
// クラス Printer
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Printer::Printer(CNFddMgrImpl* mgr,
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
Printer::print_id(CNFddEdge e)
{
  ymuint64 id = mIdMgr.id(e);
  ios::fmtflags save = mStream.flags();
  mStream << setw(9) << id;
  mStream.flags(save);
}

// e の内容を出力する．
void
Printer::print_name(CNFddEdge e)
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
Printer::print_step(CNFddEdge e)
{
  if ( e.is_leaf() ) {
    return;
  }
  CNFddEdge e2(e);
  e2.normalize();
  if ( mMark.count(e2) ) {
    return;
  }
  mMark.insert(e2);
  print_id(e2);
  CNFddNode* node = e2.get_node();
  ymuint level = node->level();
  VarId id = mMgr->varid(level);
  ios::fmtflags save = mStream.flags();
  mStream << ":IDX=" << setw(3) << id
	  << ":LVL=" << setw(3) << level
	  << ": EDGE_0=";
  mStream.flags(save);
  CNFddEdge e_0 = node->edge_0();
  print_name(e_0);
  mStream << ": EDGE_P=";
  CNFddEdge e_p = node->edge_p();
  print_name(e_p);
  mStream << ": EDGE_N=";
  CNFddEdge e_n = node->edge_n();
  print_name(e_n);
  mStream << endl;
  print_step(e_0);
  print_step(e_p);
  print_step(e_n);
}

// e を根とするZDDの内容を出力する．
void
Printer::print_root(CNFddEdge e)
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

END_NAMESPACE_YM_CNFDD
