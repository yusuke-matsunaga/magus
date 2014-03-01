
/// @file Printer2.cc
/// @brief Printer2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Printer2.h"
#include "logic/CNFdd.h"
#include "logic/CNFddMgr.h"


BEGIN_NAMESPACE_YM_CNFDD

// @brief CNFDD の内容を書き出す．
void
CNFdd::print_clause(ostream& s) const
{
  Printer2 printer(mMgr, s);
  printer.print_root(CNFddEdge(mRoot));
}


//////////////////////////////////////////////////////////////////////
// クラス Printer2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Printer2::Printer2(CNFddMgrImpl* mgr,
		   ostream& s) :
  mMgr(mgr),
  mStream(s)
{
}

// デストラクタ
Printer2::~Printer2()
{
}

// e を根とするZDDの内容を出力する．
void
Printer2::print_root(CNFddEdge e)
{
  if ( e.is_error() ) {
    mStream << "error" << endl;
  }
  else if ( e.is_overflow() ) {
    mStream << "overflow" << endl;
  }
  else {
    mElemList.clear();
    print_step(e);
    mStream << endl;
  }
}

// print_root の下請関数
void
Printer2::print_step(CNFddEdge e)
{
  if ( e.zattr() ) {
    const char* plus = "";
    for (vector<Literal>::iterator p = mElemList.begin();
	 p != mElemList.end(); ++ p) {
      Literal l = *p;
      mStream << plus << l;
      plus = " + ";
    }
    mStream << endl;
  }

  e.normalize();

  if ( e.is_zero() ) {
    return;
  }

  CNFddNode* node = e.get_node();
  ymuint level = node->level();
  VarId var = mMgr->varid(level);

  CNFddEdge e_p = node->edge_p();
  mElemList.push_back(Literal(var, false));
  print_step(e_p);
  mElemList.pop_back();

  CNFddEdge e_n = node->edge_n();
  mElemList.push_back(Literal(var, true));
  print_step(e_n);
  mElemList.pop_back();

  CNFddEdge e_0 = node->edge_0();
  print_step(e_0);
}

END_NAMESPACE_YM_CNFDD
