
/// @file libym_bdn/BdnDumper.cc
/// @brief BdnDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_bdn/BdnDumper.h"

#include "ym_bdn/BdnMgr.h"
#include "ym_bdn/BdnPort.h"
#include "ym_bdn/BdnDff.h"
#include "ym_bdn/BdnLatch.h"
#include "ym_bdn/BdnNode.h"
#include "ym_bdn/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_BDN

BEGIN_NONAMESPACE

// 出力ノードの内容を出力する．
void
dump_output(ostream& s,
	    const BdnNode* node)
{
  const BdnNode* inode = node->output_fanin();
  bool oinv = node->output_fanin_inv();
  if ( inode ) {
    // 普通のノードの場合
    if ( oinv ) {
      s << "~";
    }
    s << inode->id_str();
  }
  else {
    // 定数ノードの場合
    if ( oinv ) {
      s << "1";
    }
    else {
      s << "0";
    }
  }
}

END_NONAMESPACE

// @brief コンストラクタ
BdnDumper::BdnDumper()
{
}

// @brief デストラクタ
BdnDumper::~BdnDumper()
{
}

// @brief BdnMgr の内容をダンプする関数
// @param[in] s 出力先のストリーム
// @param[in] network 対象のネットワーク
void
BdnDumper::operator()(ostream& s,
		      const BdnMgr& network)
{
  ymuint np = network.port_num();
  for (ymuint i = 0; i < np; ++ i) {
    const BdnPort* port = network.port(i);
    s << "Port#" << port->id() << ": " << port->name() << endl;
  }

  const BdnDffList& dff_list = network.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* output = dff->output();
    const BdnNode* input = dff->input();
    const BdnNode* clock = dff->clock();
    const BdnNode* set = dff->set();
    const BdnNode* reset = dff->reset();
    s << "DFF#" << dff->id() << ": " << dff->name() << " ("
      << "OUTPUT=" << output->id_str()
      << ", INPUT=";
    dump_output(s, input);
    s << ", CLOCK=";
    dump_output(s, clock);
    if ( set->output_fanin() ) {
      s << ", SET=";
      dump_output(s, set);
    }
    if ( reset->output_fanin() ) {
      s << ", RESET=";
      dump_output(s, reset);
    }
    s << ")" << endl;
  }

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    assert_cond( node->is_input(), __FILE__, __LINE__);
    s << node->id_str() << ": ";
    switch ( node->type() ) {
    case BdnNode::kINPUT:
      s << " PORT#" << node->port()->id();
      if ( node->port()->bit_width() > 1 ) {
	s << "[" << node->port_bitpos() << "]";
      }
      break;

    case BdnNode::kDFF_OUTPUT:
      s << " OUTPUT@DFF#" << node->dff()->id();
      break;

    case BdnNode::kLATCH_OUTPUT:
      s << " OUTPUT@LATCH#" << node->latch()->id();
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    s << endl;
  }

  const BdnNodeList& lnode_list = network.lnode_list();
  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    assert_cond( node->is_logic(), __FILE__, __LINE__);
    s << node->id_str();
    s << " :  = LOGIC[";
    ymuint fcode = node->fcode();
    s << ((fcode >> 3) & 1) << ((fcode >> 2) & 1)
      << ((fcode >> 1) & 1) << (fcode & 1) << "] ("
      << " " << node->fanin0()->id_str()
      << ", " << node->fanin1()->id_str()
      << ")" << endl;
  }

  const BdnNodeList& output_list = network.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    assert_cond( node->is_output(), __FILE__, __LINE__);
    s << node->id_str() << ": ";
    switch ( node->type() ) {
    case BdnNode::kOUTPUT:
      s << "PORT#" << node->port()->id();
      if ( node->port()->bit_width() > 1 ) {
	s << "[" << node->port_bitpos() << "]";
      }
      break;

    case BdnNode::kDFF_INPUT:
      s << "INPUT@DFF#" << node->dff()->id();
      break;

    case BdnNode::kDFF_CLOCK:
      s << "CLOCK@DFF#" << node->dff()->id();
      break;

    case BdnNode::kDFF_SET:
      s << "SET@DFF#" << node->dff()->id();
      break;

    case BdnNode::kDFF_RESET:
      s << "RESET@DFF#" << node->dff()->id();
      break;

    case BdnNode::kLATCH_INPUT:
      s << "INPUT@LATCH#" << node->latch()->id();
      break;

    case BdnNode::kLATCH_ENABLE:
      s << "ENABLE@LATCH#" << node->latch()->id();
      break;

    default:
      assert_not_reached(__FILE__, __LINE__);
      break;
    }
    s << " = ";
    dump_output(s, node);
    s << endl;
  }
}

END_NAMESPACE_YM_BDN
