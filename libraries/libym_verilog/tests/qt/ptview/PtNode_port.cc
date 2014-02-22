
/// @file PtNode_port.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode_port.h"
#include "PtNode_expr.h"
#include "PtNode_misc.h"
#include "verilog/pt/PtPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス PortNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port ポートを表すパース木
PortNode::PortNode(const PtPort* port) :
  mPort(port)
{
}

// @brief デストラクタ
PortNode::~PortNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
PortNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Port";
    }
    else if ( column == 1 ) {
      if ( mPort->ext_name() ) {
	return mPort->ext_name();
      }
      else {
	return "";
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
PortNode::loc() const
{
  return mPort->file_region();
}

// @brief 子供の配列を作る．
void
PortNode::expand() const
{
  ymuint n = mPort->portref_size();
  mChildren.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    mChildren[i] = new ExprNode("PortRef", mPort->portref_elem(i));
  }
}

END_NAMESPACE_YM_VERILOG
