
/// @file EufNode.cc
/// @brief EufNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufNode.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufNode
//////////////////////////////////////////////////////////////////////

// @brief 左辺の式を得る．
// @note type() が kCon, kDis, kNeg, kEq の時のみ有効
EufNode*
EufNode::left() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 右辺の式を得る．
// @note type() が kCon, kDis, kEq の時のみ有効
EufNode*
EufNode::right() const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief 識別子名を得る．
// @note type() が kFunc, kVar の時のみ有効
string
EufNode::id_name() const
{
  assert_not_reached(__FILE__, __LINE__);
  return string();
}

// @brief 引数の数を得る．
// @note type() が kFunc の時のみ有効
ymuint
EufNode::arg_num() const
{
  assert_not_reached(__FILE__, __LINE__);
  return 0;
}

// @brief 引数を得る．
// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
// @note type() が kFunc の時のみ有効
EufNode*
EufNode::arg(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

END_NAMESPACE_YM_LLVMEQ
