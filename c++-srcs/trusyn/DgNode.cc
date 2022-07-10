
/// @file DgNode.cc
/// @brief DgNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "DgNode.h"


BEGIN_NAMESPACE_TRUSYN

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
  return DgEdge::make_zero();
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


//////////////////////////////////////////////////////////////////////
// クラス DgOrNode
//////////////////////////////////////////////////////////////////////

// @brief ORタイプの時に true を返す．
bool
DgOrNode::is_or() const
{
  return true;
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


//////////////////////////////////////////////////////////////////////
// クラス DgCplxNode
//////////////////////////////////////////////////////////////////////

// @brief CPLXタイプの時に true を返す．
bool
DgCplxNode::is_cplx() const
{
  return true;
}

END_NAMESPACE_TRUSYN
