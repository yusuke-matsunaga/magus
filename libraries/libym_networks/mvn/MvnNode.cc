
/// @file MvnNode.cc
/// @brief MvnNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/MvnNode.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

// @brief コンストラクタ
// @param[in] module 親のモジュール
MvnNode::MvnNode(MvnModule* parent) :
  mParent(parent)
{
}

// @brief デストラクタ
MvnNode::~MvnNode()
{
}

END_NAMESPACE_YM_NETWORKS_MVN
