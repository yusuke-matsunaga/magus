
/// @file libym_verilog/tests/vlview/VmNode.cc
/// @brief VmNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VmNode::VmNode() :
  mExpanded(false)
{
}

// @brief デストラクタ
VmNode::~VmNode()
{
  for (vector<VmNode*>::iterator p = mChildren.begin();
       p != mChildren.end(); ++ p) {
    delete *p;
  }
}

// @brief 子供を追加する．
void
VmNode::add_child(VmNode* node) const
{
  mChildren.push_back(node);
}


//////////////////////////////////////////////////////////////////////
// クラス VmNode1
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VmNode1::VmNode1(const VlMgr& mgr) :
  mMgr(mgr)
{
}

// @brief デストラクタ
VmNode1::~VmNode1()
{
}

// @brief VlMgr を取り出す．
const VlMgr&
VmNode1::vl_mgr() const
{
  return mMgr;
}

END_NAMESPACE_YM_VERILOG
