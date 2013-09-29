
/// @file EufEq.cc
/// @brief EufEq の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufEq.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufEq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
// @param[in] left, right 左辺と右辺の式
EufEq::EufEq(ymuint id,
	     VarId vid,
	     EufNode* left,
	     EufNode* right) :
  EufBin(id, vid, left, right)
{
}

// @brief デストラクタ
EufEq::~EufEq()
{
}

// @brief 型を得る．
EufNode::tType
EufEq::type() const
{
  return kEq;
}

// @brief Boolean 型の時 true を返す．
bool
EufEq::is_boolean() const
{
  return true;
}

END_NAMESPACE_YM_LLVMEQ
