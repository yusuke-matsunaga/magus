
/// @file MislibBop.cc
/// @brief MislibBop, MislibAnd, MislibOr, MislibXor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibBop.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
// クラス MislibBop
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibBop::MislibBop(const FileRegion& loc,
		     const MislibNode* child1,
		     const MislibNode* child2) :
  MislibNodeImpl(loc),
  mChild1(child1),
  mChild2(child2)
{
}

// デストラクタ
MislibBop::~MislibBop()
{
}

// @brief 論理式を表す型のときに true を返す．
bool
MislibBop::is_expr() const
{
  return true;
}

// 1番目の子供を取り出す．
const MislibNode*
MislibBop::child1() const
{
  return mChild1;
}

// 2番目の子供を取り出す．
const MislibNode*
MislibBop::child2() const
{
  return mChild2;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibAnd
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibAnd::MislibAnd(const FileRegion& loc,
		     const MislibNode* child1,
		     const MislibNode* child2) :
  MislibBop(loc, child1, child2)
{
}

// デストラクタ
MislibAnd::~MislibAnd()
{
}

// 種類を取り出す．
MislibNode::tType
MislibAnd::type() const
{
  return kAnd;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
Expr
MislibAnd::to_expr(const HashMap<ShString, ymuint>& name_map) const
{
  return child1()->to_expr(name_map) & child2()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibAnd::dump(ostream& s) const
{
  s << "<AND>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "<CHILD2>" << endl;
  child2()->dump(s);
  s << "</CHILD2>" << endl;

  s << "</AND>" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibOr
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibOr::MislibOr(const FileRegion& loc,
		   const MislibNode* child1,
		   const MislibNode* child2) :
  MislibBop(loc, child1, child2)
{
}

// デストラクタ
MislibOr::~MislibOr()
{
}

// 種類を取り出す．
MislibNode::tType
MislibOr::type() const
{
  return kOr;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
Expr
MislibOr::to_expr(const HashMap<ShString, ymuint>& name_map) const
{
  return child1()->to_expr(name_map) | child2()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibOr::dump(ostream& s) const
{
  s << "<OR>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "<CHILD2>" << endl;
  child2()->dump(s);
  s << "</CHILD2>" << endl;

  s << "</OR>" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス MislibXor
//////////////////////////////////////////////////////////////////////

// コンストラクタ
MislibXor::MislibXor(const FileRegion& loc,
		     const MislibNode* child1,
		     const MislibNode* child2) :
  MislibBop(loc, child1, child2)
{
}

// デストラクタ
MislibXor::~MislibXor()
{
}

// 種類を取り出す．
MislibNode::tType
MislibXor::type() const
{
  return kXor;
}

// @brief 対応する論理式を生成する．
// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
Expr
MislibXor::to_expr(const HashMap<ShString, ymuint>& name_map) const
{
  return child1()->to_expr(name_map) ^ child2()->to_expr(name_map);
}

// 内容を出力する．
// デバッグ用
void
MislibXor::dump(ostream& s) const
{
  s << "<XOR>" << endl;
  dump_loc(s);

  s << "<CHILD1>" << endl;
  child1()->dump(s);
  s << "</CHILD1>" << endl;

  s << "<CHILD2>" << endl;
  child2()->dump(s);
  s << "</CHILD2>" << endl;

  s << "</XOR>" << endl;
}

END_NAMESPACE_YM_MISLIB
