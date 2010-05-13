
/// @file libym_verilog/tests/vlview/VlPtNode.cc
/// @brief VlPtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlPtNode.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlPtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlPtNode::VlPtNode() :
  mExpanded(false)
{
}

// @brief デストラクタ
VlPtNode::~VlPtNode()
{
  for (vector<VlPtNode*>::iterator p = mChildren.begin();
       p != mChildren.end(); ++ p) {
    delete *p;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VlPtScalarNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VlPtScalarNode::VlPtScalarNode()
{
}

// @brief デストラクタ
VlPtScalarNode::~VlPtScalarNode()
{
}

// @brief 対象のファイル上での位置を返す．
// @note このクラスでは空の FileRegion を返す．
FileRegion
VlPtScalarNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
// @note このクラスではなにもしない．
void
VlPtScalarNode::expand() const
{
}

END_NAMESPACE_YM_VERILOG
