
/// @file libym_aig/AigNode.cc
/// @brief AigNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: AigNode.cc 2203 2009-04-16 05:04:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_aig/AigNode.h"


BEGIN_NAMESPACE_YM_AIG

//////////////////////////////////////////////////////////////////////
// クラス AigHandle
//////////////////////////////////////////////////////////////////////

// @brief ノードの通し番号を得る．
ymuint
AigHandle::node_id() const
{
  return node()->node_id();
}


/// @relates AigHandle
/// @brief 内容を出力する関数
ostream&
operator<<(ostream& s,
	   AigHandle src)
{
  bool inv = src.inv();
  AigNode* node = src.node();
  if ( node ) {
    if ( inv ) {
      s << "~";
    }
    s << node->node_id();
  }
  else {
    if ( inv ) {
      s << "CONST1";
    }
    else {
      s << "CONST0";
    }
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// AIG(And-Inverter Graph) のノードを表すクラス
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ノード番号
AigNode::AigNode(ymuint id) :
  mFlags(id << kSftN)
{
}

// デストラクタ
AigNode::~AigNode()
{
}

// @brief 入力タイプに設定する．
// @param[in] input_id 入力番号
void
AigNode::set_input(ymuint input_id)
{
  mFlags |= (1U << kSftT);
  mFanins[0].mPackedData = input_id;
}

// @brief ANDタイプに設定する．
// @param[in] fanin0, fanin1 ファンインのハンドル
void
AigNode::set_and(AigHandle fanin0,
		 AigHandle fanin1)
{
  mFlags &= ~(1U << kSftT);
  mFanins[0] = fanin0;
  mFanins[1] = fanin1;
}

END_NAMESPACE_YM_AIG
