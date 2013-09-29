
/// @file EufBin.cc
/// @brief EufBin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufBin.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufBin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] left, right 左辺と右辺の式
EufBin::EufBin(ymuint id,
	       EufNode* left,
	       EufNode* right) :
  EufNode(id),
  mLeft(left),
  mRight(right)
{
  mLink = NULL;
}

// @brief デストラクタ
EufBin::~EufBin()
{
}

// @brief 左辺の式を得る．
// @note type() が kCon, kDis, kNeg, kEq の時のみ有効
EufNode*
EufBin::left() const
{
  return mLeft;
}

// @brief 右辺の式を得る．
// @note type() が kCon, kDis, kEq の時のみ有効
EufNode*
EufBin::right() const
{
  return mRight;
}

END_NAMESPACE_YM_LLVMEQ
