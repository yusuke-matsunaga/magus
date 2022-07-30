
/// @file DgNode.cc
/// @brief DgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DgNode.h"
#include "DgEdge.h"


BEGIN_NAMESPACE_DG

//////////////////////////////////////////////////////////////////////
// クラス DgNode
//////////////////////////////////////////////////////////////////////

// @brief LITタイプの時に true を返す．
bool
DgNode::is_lit() const
{
  return false;
}

// @brief ORタイプの時に true を返す．
bool
DgNode::is_or() const
{
  return false;
}

// @brief XORタイプの時に true を返す．
bool
DgNode::is_xor() const
{
  return false;
}

// @brief CPLXタイプの時に true を返す．
bool
DgNode::is_cplx() const
{
  return false;
}

// @brief subfunction の数を得る．
SizeType
DgNode::child_num() const
{
  return 0;
}

// @brief subfunction を表す枝を得る．
DgEdge
DgNode::child(
  SizeType pos
) const
{
  ASSERT_NOT_REACHED;
  return DgEdge::zero();
}

// @brief print の共通部分
void
DgNode::print_base(
  ostream& s,
  const string& type
) const
{
  s << "#" << id() << ": " << type;
}


//////////////////////////////////////////////////////////////////////
// クラス DgLitNode
//////////////////////////////////////////////////////////////////////

// @brief LITタイプの時に true を返す．
bool
DgLitNode::is_lit() const
{
  return true;
}

// @brief 内容を出力する．
void
DgLitNode::print(
  ostream& s
) const
{
  print_base(s, "LIT");
  s << "(" << top() << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス DgMidNode
//////////////////////////////////////////////////////////////////////

// @brief subfunction の数を得る．
SizeType
DgMidNode::child_num() const
{
  return mChildList.size();
}

// @brief subfunction を表す枝を得る．
DgEdge
DgMidNode::child(
  SizeType pos
) const
{
  ASSERT_COND( 0 <= pos && pos < child_num() );
  return mChildList[pos];
}

// @brief pint() の下請け処理を行う．
void
DgMidNode::print_sub(
  ostream& s,
  const string& type
) const
{
  print_base(s, type);
  s << "(";
  const char* comma = "";
  for ( SizeType i = 0; i < child_num(); ++ i ) {
    auto cedge = child(i);
    s << comma;
    comma = ", ";
    cedge.print(s);
  }
  s << ")" << endl;
}


//////////////////////////////////////////////////////////////////////
// クラス DgOrNode
//////////////////////////////////////////////////////////////////////

// @brief ORタイプの時に true を返す．
bool
DgOrNode::is_or() const
{
  return true;
}

// @brief 内容を出力する．
void
DgOrNode::print(
  ostream& s
) const
{
  print_sub(s, "OR");
}


//////////////////////////////////////////////////////////////////////
// クラス DgXorNode
//////////////////////////////////////////////////////////////////////

// @brief XORタイプの時に true を返す．
bool
DgXorNode::is_xor() const
{
  return true;
}

// @brief 内容を出力する．
void
DgXorNode::print(
  ostream& s
) const
{
  print_sub(s, "XOR");
}


//////////////////////////////////////////////////////////////////////
// クラス DgCplxNode
//////////////////////////////////////////////////////////////////////

// @brief CPLXタイプの時に true を返す．
bool
DgCplxNode::is_cplx() const
{
  return true;
}

// @brief 内容を出力する．
void
DgCplxNode::print(
  ostream& s
) const
{
  print_sub(s, "CPLX");
}

END_NAMESPACE_DG
