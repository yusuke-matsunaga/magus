
/// @file BdnDumper.cc
/// @brief BdnDumper の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmNetworks/BdnDumper.h"

#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnDff.h"
#include "YmNetworks/BdnLatch.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnNodeHandle.h"


BEGIN_NAMESPACE_YM_NETWORKS_BDN

BEGIN_NONAMESPACE

// ポートの名前を返す．
string
port_idstr(const BdnPort* port)
{
  ostringstream buf;
  buf << "PORT[" << port->id() << "]";
  return buf.str();
}

// D-FF の名前を返す．
string
dff_idstr(const BdnDff* dff)
{
  ostringstream buf;
  buf << "DFF[" << dff->id() << "]";
  return buf.str();
}

// ラッチの名前を返す．
string
latch_idstr(const BdnLatch* latch)
{
  ostringstream buf;
  buf << "LATCH[" << latch->id() << "]";
  return buf.str();
}

// ノードの名前を返す．
string
node_idstr(const BdnNode* node)
{
  ostringstream buf;
  if ( node->is_input() ) {
    buf << 'I';
  }
  else if ( node->is_logic() ) {
    buf << 'L';
  }
  else if ( node->is_output() ) {
    buf << 'O';
  }
  else {
    buf << 'X';
  }
  buf << "[" << node->id() << "]";
  return buf.str();
}

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
    s << node_idstr(inode);
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
    s << port_idstr(port) << " : " << port->name() << endl;
  }

  const BdnDffList& dff_list = network.dff_list();
  for (BdnDffList::const_iterator p = dff_list.begin();
       p != dff_list.end(); ++ p) {
    const BdnDff* dff = *p;
    const BdnNode* output = dff->output();
    const BdnNode* input = dff->input();
    const BdnNode* clock = dff->clock();
    const BdnNode* clear = dff->clear();
    const BdnNode* preset = dff->preset();
    s << dff_idstr(dff) << " : " << dff->name() << " ("
      << "OUTPUT=" << node_idstr(output)
      << ", INPUT=";
    dump_output(s, input);
    s << ", CLOCK=";
    dump_output(s, clock);
    if ( clear->output_fanin() ) {
      s << ", CLEAR=";
      dump_output(s, clear);
    }
    if ( preset->output_fanin() ) {
      s << ", PRESET=";
      dump_output(s, preset);
    }
    s << ")" << endl;
  }

  const BdnLatchList& latch_list = network.latch_list();
  for (BdnLatchList::const_iterator p = latch_list.begin();
       p != latch_list.end(); ++ p) {
    const BdnLatch* latch = *p;
    const BdnNode* output = latch->output();
    const BdnNode* input = latch->input();
    const BdnNode* enable = latch->enable();
    const BdnNode* clear = latch->clear();
    const BdnNode* preset = latch->preset();
    s << latch_idstr(latch) << " : " << latch->name() << " ("
      << "OUTPUT = " << node_idstr(output)
      << ", INPUT = ";
    dump_output(s, input);
    if ( enable->output_fanin() ) {
      s << ", ENABLE = ";
      dump_output(s, enable);
    }
    if ( clear->output_fanin() ) {
      s << ", CLEAR = ";
      dump_output(s, clear);
    }
    if ( preset->output_fanin() ) {
      s << ", PRESET = ";
      dump_output(s, preset);
    }
    s << ")" << endl;
  }

  const BdnNodeList& input_list = network.input_list();
  for (BdnNodeList::const_iterator p = input_list.begin();
       p != input_list.end(); ++ p) {
    const BdnNode* node = *p;
    ASSERT_COND( node->is_input() );
    s << node_idstr(node) << ": ";
    switch ( node->input_type() ) {
    case BdnNode::kPRIMARY_INPUT:
      s << " " << port_idstr(node->port());
      if ( node->port()->bit_width() > 1 ) {
	s << "[" << node->port_bitpos() << "]";
      }
      break;

    case BdnNode::kDFF_OUTPUT:
      s << " OUTPUT@" << dff_idstr(node->dff());
      break;

    case BdnNode::kLATCH_OUTPUT:
      s << " OUTPUT@" << latch_idstr(node->latch());
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    s << endl;
  }

  const BdnNodeList& lnode_list = network.lnode_list();
  for (BdnNodeList::const_iterator p = lnode_list.begin();
       p != lnode_list.end(); ++ p) {
    const BdnNode* node = *p;
    ASSERT_COND( node->is_logic() );
    s << node_idstr(node);
    s << " :  = LOGIC( ";
    const char* f0 = node->fanin0_inv() ? "~" : "";
    const char* f1 = node->fanin1_inv() ? "~" : "";
    const char* op = node->is_xor() ? "^" : "&";
    s << f0 << node_idstr(node->fanin0())
      << " " << op << " "
      << f1 << node_idstr(node->fanin1())
      << " )" << endl;
  }

  const BdnNodeList& output_list = network.output_list();
  for (BdnNodeList::const_iterator p = output_list.begin();
       p != output_list.end(); ++ p) {
    const BdnNode* node = *p;
    ASSERT_COND( node->is_output() );
    s << node_idstr(node) << ": ";
    switch ( node->output_type() ) {
    case BdnNode::kPRIMARY_OUTPUT:
      s << "PORT#" << node->port()->id();
      if ( node->port()->bit_width() > 1 ) {
	s << "[" << node->port_bitpos() << "]";
      }
      break;

    case BdnNode::kDFF_DATA:
      s << "DATA@" << dff_idstr(node->dff());
      break;

    case BdnNode::kDFF_CLOCK:
      s << "CLOCK@" << dff_idstr(node->dff());
      break;

    case BdnNode::kDFF_CLEAR:
      s << "CLEAR@" << dff_idstr(node->dff());
      break;

    case BdnNode::kDFF_PRESET:
      s << "PRESET@" << dff_idstr(node->dff());
      break;

    case BdnNode::kLATCH_DATA:
      s << "DATA@" << latch_idstr(node->latch());
      break;

    case BdnNode::kLATCH_ENABLE:
      s << "ENABLE@" << latch_idstr(node->latch());
      break;

    case BdnNode::kLATCH_CLEAR:
      s << "CLEAR@" << latch_idstr(node->latch());
      break;

    case BdnNode::kLATCH_PRESET:
      s << "PRESET@" << latch_idstr(node->latch());
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    s << " = ";
    dump_output(s, node);
    s << endl;
  }
}

END_NAMESPACE_YM_NETWORKS_BDN
