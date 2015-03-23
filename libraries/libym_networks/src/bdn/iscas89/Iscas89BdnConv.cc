
/// @file Iscas89BdnConv
/// @brief Iscas89BdnConv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Iscas89BdnConv.h"
#include "Iscas89Network.h"

#include "YmNetworks/BdnMgr.h"
#include "YmNetworks/BdnPort.h"
#include "YmNetworks/BdnNode.h"
#include "YmNetworks/BdnNodeHandle.h"
#include "YmNetworks/BdnDff.h"


BEGIN_NAMESPACE_YM_ISCAS89

//////////////////////////////////////////////////////////////////////
// Iscas89BdnConv
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Iscas89BdnConv::Iscas89BdnConv()
{
}

// @brief デストラクタ
Iscas89BdnConv::~Iscas89BdnConv()
{
}

// @brief 変換する．
// @param[in] iscas89_network 変換元のネットワーク
// @param[in] network 変換先のネットワーク
// @param[in] clock_name クロック信号のポート名
bool
Iscas89BdnConv::operator()(const Iscas89Network& iscas89_network,
			   BdnMgr& network,
			   const string& clock_name)
{
  mNetwork = &network;
  mNetwork->clear();

  ymuint n = iscas89_network.max_node_id();
  mNodeMap.clear();
  mNodeMap.resize(n);
  mNodeFlag.clear();
  mNodeFlag.resize(n, false);

  // 外部入力ノードの生成
  ymuint npi = iscas89_network.npi();
  for (ymuint i = 0; i < npi; ++ i) {
    const Iscas89Node* iscas89_node = iscas89_network.pi(i);
    BdnPort* port = mNetwork->new_input_port(iscas89_node->name(), 1);
    BdnNode* node = port->_input(0);
    put_node(iscas89_node, BdnNodeHandle(node, false));
  }

  // D-FF の生成
  ymuint nff = iscas89_network.nff();
  vector<BdnDff*> dff_array(nff);
  BdnNodeHandle clock_h;
  if ( nff > 0 ) {
    // クロック用の外部入力の生成
    BdnPort* clock_port = mNetwork->new_input_port(clock_name, 1);
    BdnNode* clock = clock_port->_input(0);
    clock_h = BdnNodeHandle(clock, false);
  }
  for (ymuint i = 0; i < nff; ++ i) {
    const Iscas89Node* iscas89_node = iscas89_network.ff(i);
    BdnDff* dff = mNetwork->new_dff(iscas89_node->name());
    dff_array[i] = dff;

    // D-FF の出力の登録
    BdnNode* node = dff->_output();
    put_node(iscas89_node, BdnNodeHandle(node, false));

    // クロック信号の設定
    BdnNode* dff_clock = dff->_clock();
    mNetwork->change_output_fanin(dff_clock, clock_h);
  }

  // 外部出力に用いられているノードを再帰的に生成
  ymuint npo = iscas89_network.npo();
  for (ymuint i = 0; i < npo; ++ i) {
    const Iscas89Node* iscas89_node = iscas89_network.po(i);
    BdnPort* port = mNetwork->new_output_port(iscas89_node->name(), 1);
    BdnNode* node = port->_output(0);
    BdnNodeHandle inode_h = make_node(iscas89_node);
    mNetwork->change_output_fanin(node, inode_h);
  }

  // D-FF に用いられているノードを再帰的に生成
  for (ymuint i = 0; i < nff; ++ i) {
    const Iscas89Node* iscas89_node = iscas89_network.ff(i);
    BdnNodeHandle inode_h = make_node(iscas89_node->fanin(0));
    BdnDff* dff = dff_array[i];
    BdnNode* dff_input = dff->_input();
    mNetwork->change_output_fanin(dff_input, inode_h);
  }

  return true;
}

// @brief iscas89_node に対応した BdnNode を生成する．
BdnNodeHandle
Iscas89BdnConv::make_node(const Iscas89Node* iscas89_node)
{
  BdnNodeHandle node_handle;
  if ( !get_node(iscas89_node, node_handle) ) {
    ASSERT_COND( iscas89_node->type() == Iscas89Node::kGate );
    ymuint ni = iscas89_node->fanin_num();
    vector<BdnNodeHandle> fanins(ni);
    for (ymuint i = 0; i < ni; ++ i) {
      fanins[i] = make_node(iscas89_node->fanin(i));
    }

    switch ( iscas89_node->gate_type() ) {
    case kGt89BUFF:
      node_handle = fanins[0];
      break;

    case kGt89NOT:
      node_handle = ~fanins[0];
      break;

    case kGt89AND:
      node_handle = mNetwork->new_and(fanins);
      break;

    case kGt89NAND:
      node_handle = mNetwork->new_nand(fanins);
      break;

    case kGt89OR:
      node_handle = mNetwork->new_or(fanins);
      break;

    case kGt89NOR:
      node_handle = mNetwork->new_nor(fanins);
      break;

    case kGt89XOR:
      node_handle = mNetwork->new_xor(fanins);
      break;

    case kGt89XNOR:
      node_handle = mNetwork->new_xnor(fanins);
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    put_node(iscas89_node, node_handle);
  }
  return node_handle;
}

// @brief iscas89_node に対応した BdnNode を取り出す．
bool
Iscas89BdnConv::get_node(const Iscas89Node* iscas89_node,
			 BdnNodeHandle& node_handle)
{
  ymuint id = iscas89_node->id();
  if ( mNodeFlag[id] ) {
    node_handle = mNodeMap[id];
    return true;
  }
  return false;
}

// @brief iscas89_node に対応した BdnNode を登録する．
void
Iscas89BdnConv::put_node(const Iscas89Node* iscas89_node,
			 BdnNodeHandle node_handle)
{
  ymuint id = iscas89_node->id();
  mNodeMap[id] = node_handle;
  mNodeFlag[id] = true;
}

END_NAMESPACE_YM_ISCAS89
