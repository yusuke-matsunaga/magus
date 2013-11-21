
/// @file libym_mislib/MislibSymbol.cc
/// @brief MislibNoinv, MislibInv, MislibUnknown の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MislibSymbol.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibNoinv
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibNoninv::MislibNoninv(const FileRegion& loc) :
  MislibNodeImpl(loc)
{
}

// デストラクタ
MislibNoninv::~MislibNoninv()
{
}

// 種類を取り出す．
MislibNode::tType
MislibNoninv::type() const
{
  return kNoninv;
}

// 内容を出力する．
// デバッグ用
void
MislibNoninv::dump(ostream& s) const
{
  s << "<NONINV>" << endl;
  dump_loc(s);
  s << "</NONINV>" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibInv
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibInv::MislibInv(const FileRegion& loc) :
  MislibNodeImpl(loc)
{
}

// デストラクタ
MislibInv::~MislibInv()
{
}

// 種類を取り出す．
MislibNode::tType
MislibInv::type() const
{
  return kInv;
}

// 内容を出力する．
// デバッグ用
void
MislibInv::dump(ostream& s) const
{
  s << "<INV>" << endl;
  dump_loc(s);
  s << "</INV>" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibUnknown
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibUnknown::MislibUnknown(const FileRegion& loc) :
  MislibNodeImpl(loc)
{
}

// デストラクタ
MislibUnknown::~MislibUnknown()
{
}

// 種類を取り出す．
MislibNode::tType
MislibUnknown::type() const
{
  return kUnknown;
}

// 内容を出力する．
// デバッグ用
void
MislibUnknown::dump(ostream& s) const
{
  s << "<UNKNOWN>" << endl;
  dump_loc(s);
  s << "</UNKNOWN>" << endl;
}

END_NAMESPACE_YM_MISLIB
