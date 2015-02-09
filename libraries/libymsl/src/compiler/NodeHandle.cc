
/// @file NodeHandle.cc
/// @brief NodeHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NodeHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス NodeHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] node ノード
NodeHandle::NodeHandle(ShString name,
		       IrNode* node) :
  mName(name),
  mNode(node)
{
}

// @brief デストラクタ
NodeHandle::~NodeHandle()
{
}

// @brief 名前を返す．
ShString
NodeHandle::name() const
{
  return mName;
}

// @brief ノードを返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
NodeHandle::node() const
{
  return mNode;
}

END_NAMESPACE_YM_YMSL
