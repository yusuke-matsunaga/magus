
/// @file ConstHandle.cc
/// @brief ConstHandle の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ConstHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス ConstHandle
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] name 名前
// @param[in] const_node 定数ノード
ConstHandle::ConstHandle(ShString name,
			 IrNode* const_node) :
  mName(name),
  mConstNode(const_node)
{
}

// @brief デストラクタ
ConstHandle::~ConstHandle()
{
}

// @brief 名前を返す．
ShString
ConstHandle::name() const
{
  return mName;
}

// @brief 定数ノードを返す．
//
// 他の要素の場合には NULL を返す．
IrNode*
ConstHandle::const_node() const
{
  return mConstNode;
}

END_NAMESPACE_YM_YMSL
