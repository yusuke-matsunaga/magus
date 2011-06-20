
/// @file libym_mislib/MislibNum.cc
/// @brief MislibNum の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibPtImpl1.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNum.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibNum
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibNum::MislibNum(const FileRegion& loc,
		     double num) :
  MislibNodeImpl(loc),
  mNum(num)
{
}

// デストラクタ
MislibNum::~MislibNum()
{
}

// 種類を取り出す．
MislibNode::tType
MislibNum::type() const
{
  return kNum;
}

// 数値を取り出す
double
MislibNum::num() const
{
  return mNum;
}

// 内容を出力する．
// デバッグ用
void
MislibNum::dump(ostream& s) const
{
  s << "<NUM>" << endl;
  dump_loc(s);
  s << num() << endl
    << "</NUM>" << endl;
}

END_NAMESPACE_YM_MISLIB
