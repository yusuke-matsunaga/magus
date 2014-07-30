
/// @file Printer2.cc
/// @brief Printer2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "Printer2.h"
#include "YmLogic/Zdd.h"
#include "YmLogic/ZddMgr.h"


BEGIN_NAMESPACE_YM_ZDD

// @brief ZDD の内容を書き出す．
void
Zdd::print_set(ostream& s) const
{
  Printer2 printer(mMgr, s);
  printer.print_root(ZddEdge(mRoot));
}


//////////////////////////////////////////////////////////////////////
// クラス Printer2
//////////////////////////////////////////////////////////////////////

// コンストラクタ
Printer2::Printer2(ZddMgrImpl* mgr,
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
Printer2::print_root(ZddEdge e)
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
Printer2::print_step(ZddEdge e)
{
  if ( e.zattr() ) {
    mStream << "{";
    for (vector<ymuint32>::iterator p = mElemList.begin();
	 p != mElemList.end(); ++ p) {
      ymuint32 l = *p;
      mStream << " " << mMgr->varid(l);
    }
    mStream << "}" << endl;
  }

  e.normalize();

  if ( e.is_zero() ) {
    return;
  }

  ZddNode* node = e.get_node();
  ZddEdge e0 = node->edge0();
  print_step(e0);

  ZddEdge e1 = node->edge1();
  ymuint level = node->level();
  mElemList.push_back(level);
  print_step(e1);
  mElemList.pop_back();
}

END_NAMESPACE_YM_ZDD
