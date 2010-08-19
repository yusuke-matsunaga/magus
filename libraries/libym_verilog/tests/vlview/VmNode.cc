
/// @file libym_verilog/tests/vlview/VmNode.cc
/// @brief VmNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmNode.h"
#include "VmMiscNode.h"


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

// @brief 文字列型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_child(const QString& label,
		  const QString& value) const
{
  add_child( new VmStrNode(label, value) );
}

// @brief ブール型のノードを追加する．
// @param[in] label ラベル
// @param[in] value 値
void
VmNode::add_child(const QString& label,
		  bool value) const
{
  add_child( new VmBoolNode(label, value) );
}

END_NAMESPACE_YM_VERILOG
